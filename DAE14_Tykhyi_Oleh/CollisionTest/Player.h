#pragma once
#include "Entity.h"

class Map;
class ParticleManager;

class InteractableObject;
class ThrowableObject;

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
		hurtFly,
		hurtOnGround,
		catPet,

		count
	};

	Player(Sprite* sprite, Sprite* splashSprite, const std::vector<AnimationFrameInfo>& playerAnimation, const Vector2f& position, float speed, float scale, int floor);

	void Draw() const override;

	void Update(float elapsedSec, Map* pMap, const Uint8* pStates, const Rectf& viewport);

	void SetState(PlayerState state);

	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e, const Vector2f& offset, ParticleManager* particleManager, const Rectf& viewport);

	void Kill(const Vector2f& impulse) override;

private:
	PlayerState m_State;
	
	std::vector<AnimationFrameInfo> m_PlayerSpriteFrames{};
	const std::vector<Vector2f> m_BaseSplashHitbox{
		Vector2f{0.f, 10.f},
		Vector2f{0.f, -10.f},
		Vector2f{100.f, -20.f},
		Vector2f{100.f, 20.f}
	};

	Sprite* m_SplashSprite;

	const float
		m_FallSpeedUp	{ 2000.f },
		m_RollSpeed		{ 500.f },
		m_JumpImpulse	{ 450.f },
		m_FrictionForce	{ 1200.f },
		m_AirResistance	{ 2000.f },
		m_VelEps		{ 0.1f },
		m_BaseDashForce	{950.f};

	const float
		m_RollCooldown{ 0.4f },
		m_AttackCooldown{ 0.25f };

	float
		m_RollCooldownTimer{0.f},
		m_AttackCooldownTimer{0.f};

	int
		m_AirAttackCount{};


#pragma region interaction
	bool
		m_IsAutoWalking{ false };

	InteractableObject*
		m_pTargetObject{};
	ThrowableObject*
		m_pHeldObject{};

	const float
		m_ArrivalThreshold{ 5.f };
#pragma endregion interaction
	void DrawSplash() const;
	
	void UpdateCurrentState(float elapsedSec);
	void ProcessStateChange(bool isMoving, bool roll, bool crouch);

	PlayerState GetNextState(bool isMoving, bool roll, bool crouch);
	PlayerState GetNextAirState();
	PlayerState GetNextGroundState(bool isMoving, bool roll, bool crouch);

	void HandleKeyboard(Map* pMap, const Uint8* pStates, float elapsedSec);

	void HandleAutowalk();

	void Interact(Map* pMap);
	void Attack(const Vector2f& mousePos, ParticleManager* particleManager);

	void SpawnAttackParticle(ParticleManager* particleManager) const;
	void SpawnThrownObject(ParticleManager* particleManager, const Vector2f& mousePos) const;

	void AttackDash(const Vector2f& mousePos);

	void ProcessJumpThroughPlatform(bool downButton);
	void HandleVerticalMovement(bool down, bool jump, float elapsedSec);
	void HandleHorizontalMovement(bool left, bool right, bool down, float elapsedSec);

	void ApplyFriction(float elapsedSec);
	void ApplyAirResistance(float elapsedSec);

	float CalculateSplashRotation(const Vector2f& mouseVec) const;

	void UpdateAttackState(float elapsedSec);

	void UpdateCooldowns(float elapsedSec);

};

