#pragma once
#include "Entity.h"
#include <vector>


class Map;
class ParticleManager;
class SoundManager;
class InteractableObject;
class ThrowableObject;
struct PlayerSnapshot;


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

	Player(
		Sprite* sprite,
		Sprite* splashSprite,
		Sprite* rangeProjectileSprite, 
		const std::vector<AnimationFrameInfo>& playerAnimation,
		const std::vector<AnimationFrameInfo>& rangeProjectileSpriteFrameInfo, 
		const Vector2f& position, 
		float speed, 
		float scale, 
		int floor
	);

	void Draw() const override;

	void Update(float elapsedSec, Map* pMap, const Uint8* pStates, const Rectf& viewport, ParticleManager* pParticleManager, SoundManager* pSoundManager);

	void SetState(PlayerState state, ParticleManager* pParticleManager, SoundManager* pSoundManager);
	PlayerState GetState() const;

	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e, const Vector2f& offset, ParticleManager* particleManager, SoundManager* pSoundManager, const Rectf& viewport);

	void Kill(const Vector2f& impulse) override;

	ThrowableObject* GetHeldObject() const;

	void Reset() override;

	bool CanDeflect() const;
	bool IsInsensible() const;

	void ApplySnapshot(const PlayerSnapshot* snapshot);

	void SetCanFinish(bool canFinish);
	bool CanFinish() const;

	bool IsSplashDrawn() const;
	int GetSplashAnimationFrame() const;
	float GetSplashRotation() const;
private:
	PlayerState m_State;
	
	std::vector<AnimationFrameInfo> m_PlayerSpriteFrames{};
	const std::vector<Vector2f> m_BaseSplashHitbox{
		Vector2f{0.f, 10.f},
		Vector2f{0.f, -10.f},
		Vector2f{100.f, -20.f},
		Vector2f{100.f, 20.f}
	};
	const std::vector<Vector2f> m_BaseRangeRangeProjectileHitbox{
		Vector2f{ -20.0f, -10.0f }, 
		Vector2f{  20.0f, -10.0f }, 
		Vector2f{  20.0f,  10.0f }, 
		Vector2f{ -20.0f,  10.0f }
	};

	Sprite*
		m_SplashSprite;

	Sprite*
		m_RangeProjectileSprite{};

	const float
		m_FallSpeedUp{ 2000.f },
		m_RollSpeed{ 500.f },
		m_JumpImpulse{ 450.f },
		m_FrictionForce{ 1200.f },
		m_AirResistance{ 2000.f },
		m_VelEps{ 0.1f },
		m_BaseDashForce{ 950.f },
		m_RangeProjectileSpeed{ 1200.f };

	const float
		m_RollCooldown{ 0.4f },
		m_AttackCooldown{ 0.25f },
		m_RangeAttackCooldown{ 1.f };

	float
		m_RollCooldownTimer{ 0.f },
		m_AttackCooldownTimer{ 0.f },
		m_RangeAttackCooldownTimer{ 0.f };

	int
		m_AirAttackCount{};

	bool
		m_CanFinish{false};

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
	void ProcessStateChange(bool isMoving, bool roll, bool crouch, ParticleManager* pParticleManager, SoundManager* pSoundManager);

	PlayerState GetNextState(bool isMoving, bool roll, bool crouch);
	PlayerState GetNextAirState();
	PlayerState GetNextGroundState(bool isMoving, bool roll, bool crouch);

	void HandleKeyboard(Map* pMap, const Uint8* pStates, float elapsedSec, ParticleManager* pParticleManager, SoundManager* pSoundManager);

	void HandleAutowalk(SoundManager* pSoundManager);

	void Interact(Map* pMap, ParticleManager* pParticleManager, SoundManager* pSoundManager);
	void Attack(const Vector2f& mousePos, ParticleManager* particleManager, SoundManager* pSoundManager);

	void SpawnAttackParticle(ParticleManager* particleManager) const;
	void SpawnRangeAttackParticle(ParticleManager* particleManager) const;

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

