#pragma once
#include "Entity.h"

#include <vector>

class Player final : public Entity
{
public:
	enum class PlayerState {
		none=-1, 

		staying,
		beforeRun,
		afterRun,
		run,
		roll,
		jump,
		crouch,
		postCrouch,
		fall,
		attack,
		attackSplash,

		count
	};

	Player(Sprite* sprite, Sprite* splashSprite, const std::vector<AnimationFrameInfo>& playerAnimation, const Vector2f& position, float speed);

	void Draw() const override;

	void Update(float elapsedSec, const Uint8* pStates, const Rectf& viewport);

	void SetState(PlayerState state);

	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e, const Rectf& viewport);

	std::vector<Vector2f> GetSplashHitbox() const;
private:
	PlayerState m_State;
	
	std::vector<AnimationFrameInfo> m_PlayerSpriteFrames{};
	std::vector<Vector2f>			m_SplashHitboxPoints{};

	Sprite* m_SplashSprite;

	const float
		m_FallSpeedUp	{ 2000.f },
		m_RollSpeed		{ 500.f },
		m_JumpImpulse	{ 450.f },
		m_FrictionForce	{ 1200.f },
		m_AirResistance	{ 2000.f },
		m_VelEps		{ 0.1f };


	void DrawSplash() const;
	
	void UpdateCurrentState(float elapsedSec);
	void ProcessStateChange(bool isMoving, bool roll, bool crouch);

	PlayerState GetNextState(bool isMoving, bool roll, bool crouch);
	PlayerState GetNextAirState();
	PlayerState GetNextGroundState(bool isMoving, bool roll, bool crouch);

	void HandleKeyboard(const Uint8* pStates, float elapsedSec);

	void Attack(const Vector2f& mousePos);
	void AttackDash(const Vector2f& mousePos);

	void HandleVerticalMovement(bool down, bool jump, float elapsedSec);
	void HandleHorizontalMovement(bool left, bool right, bool down, float elapsedSec);

	void ApplyFriction(float elapsedSec);
	void ApplyAirResistance(float elapsedSec);

	float CalculateSplashRotation(const Vector2f& mouseVec);

	void UpdateSplashHitbox();
	void UpdateAttackState(float elapsedSec);


	void DrawHitboxes() const;
};

