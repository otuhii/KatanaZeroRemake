#pragma once
#include "Entity.h"
#include "ControlPoint.h"

#include <vector>

class ParticleManager;

class Enemy : public Entity
{
public:
	enum class EnemyType {
		grunt,
		gangster,

		count
	};

	enum class EnemyState {
		idle,
		walk,
		run,
		attack,
		turn
	};

	Enemy(
		EnemyType type,
		EnemyState state,
		Sprite* pSprite, 
		const std::vector<AnimationFrameInfo>* playerAnimation, 
		const Vector2f& position, 
		float speed,
		float scale,
		int floor,
		float playerDetectionRange,
		float attackRange
	);

	virtual ~Enemy() = default;

	virtual void Draw() const override;

	virtual void Update(float elapsedSec, const Vector2f& playerPos, int playerFloor, ParticleManager* particleManager, const Rectf& viewport);

	void UpdateControlPoints(const std::vector<ControlPoint>* controlPoint);

	EnemyType GetType() const;
protected:
	bool CanSeePlayer(const Vector2f& playerPos, int playerFloor);
	bool IsPlayerInAttackRange(const Vector2f& playerPos);

	virtual void Attack(const Vector2f& playerPos, ParticleManager* particleManager) = 0;

	EnemyState GetState() const;
	float GetAttackCooldownTimer() const;
	void ResetAttackCooldownTimer();
private:
	EnemyState m_State;
	EnemyType  m_Type;

	const std::vector<AnimationFrameInfo>* m_EnemySpriteFrames{};

	const std::vector<ControlPoint>*	   m_ControlPoints{};
	std::vector<int>				       m_Path{};
	
	int 
		m_CurrentTargetControlPoint{ 0 };

	const float
		m_AttackCooldown{1.f};
	
	float
		m_AttackCooldownTimer{};

	const float
		m_PatrolSpeedMultiplier{ 1.f },
		m_RunningMultiplier{ 1.5f };

	const float
		m_DetectionRange{ 400.f },
		m_AttackRange{ 50.f };
		//m_AngleFieldOfDetection{ 90.f };

	void UpdateSprite();

	void SetState(EnemyState state);

	void UpdateCurrentState(float elapsedSec, const Vector2f& playerPos, int playerFloor, ParticleManager* particleManager);

	void UpdateIdle(float elapsedSec, const Vector2f& playerPos, int playerFloor);
	void UpdateWalk(float elapsedSec, const Vector2f& playerPos, int playerFloor);
	void UpdateRun(float elapsedSec, const Vector2f& playerPos, int playerFloor);
	virtual void UpdateAttack(float elapsedSec, const Vector2f& playerPos, ParticleManager* particleManager);
	void UpdateTurn(float elapsedSec);

	void StateInitialization(EnemyState state);

	void Patrol(float elapsedSec);
	void Chase(float elapsedSec, const Vector2f& playerPos, int playerFloor);
	
	bool MoveTo(const ControlPoint& controlPoint, float speedMultiplier);

	void UpdateCooldowns(float elapsedSec);
};

