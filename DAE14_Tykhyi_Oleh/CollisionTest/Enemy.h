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
		turn,
		dead
	};

	Enemy(
		EnemyType type,
		EnemyState state,
		Sprite* pSprite, 
		const Entity* pTarget,
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

	virtual void Update(float elapsedSec, ParticleManager* particleManager, const Rectf& viewport);

	void UpdateControlPoints(const std::vector<ControlPoint>* controlPoint);

	EnemyType		GetType() const;
	const Entity*	GetTarget();

	void SetState(EnemyState state);

	void Kill(const Vector2f& impulse) override;

protected:
	bool CanSeeTarget();
	bool IsTargetInAttackRange();

	virtual void Attack( ParticleManager* particleManager) = 0;

	EnemyState GetState() const;
	
	float GetAttackCooldown() const;
	void ResetAttackCooldown() ;

	void			UpdateIdle(float elapsedSec);
	void			UpdateWalk(float elapsedSec);
	void			UpdateRun(float elapsedSec);
	virtual void	UpdateAttack(float elapsedSec, ParticleManager* particleManager);
	void			UpdateTurn(float elapsedSec);

	void StateInitialization(EnemyState state);

	void Patrol(float elapsedSec);
	void Chase(float elapsedSec);

	bool MoveTo(const ControlPoint& controlPoint, float speedMultiplier);
private:
	EnemyState m_State;
	EnemyType  m_Type;

	const Entity* m_pTarget{};

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
		m_RunningMultiplier{ 2.f };

	const float
		m_DetectionRange{ 400.f },
		m_AttackRange{ 50.f };

	void			UpdateSprite();

	virtual void	UpdateCurrentState(float elapsedSec, ParticleManager* particleManager);

	void			UpdateCooldowns(float elapsedSec);
};

