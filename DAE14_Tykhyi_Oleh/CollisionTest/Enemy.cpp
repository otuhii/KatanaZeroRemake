#include "pch.h"
#include "Enemy.h"

#include "PathFinder.h"
#include "Map.h"

#include "utils.h"

#include "VFX.h"

Enemy::Enemy(
	EnemyType type, 
	EnemyState state,
	Sprite* pSprite, 
	const Entity* pTarget,
	const std::vector<AnimationFrameInfo>* enemyAnimationFrames,
	const Vector2f& position,
	float speed, 
	float scale, 
	int floor, 
	float playerDetectionRange, 
	float attackRange
)
	:  
	Entity(pSprite, EntityType::enemy, position, Vector2f{0.f, 0.f}, speed, floor),
	m_EnemySpriteFrames{ enemyAnimationFrames },
	m_State{state},
	m_Type{type},
	m_AttackRange{attackRange},
	m_DetectionRange{playerDetectionRange},
	m_pTarget{pTarget}
{
	GetSprite()->SetAnimationFrameInfo((*m_EnemySpriteFrames)[static_cast<int>(m_State)]);
	GetSprite()->SetScale(scale);

	UpdateHitbox();
}

void Enemy::Draw() const
{
	Entity::Draw();
}

void Enemy::UpdateControlPoints(const std::vector<ControlPoint>* controlPoints)
{
	m_ControlPoints = controlPoints;
}

Enemy::EnemyType Enemy::GetType() const
{
	return m_Type;
}

void Enemy::Update(float elapsedSec, ParticleManager* particleManager, const Map* pMap, const Rectf& viewport)
{
	Entity::Update(elapsedSec, viewport);

	if (IsAlive())
	{
		UpdateCooldowns(elapsedSec);
		UpdateCurrentState(elapsedSec, particleManager, pMap);

		UpdateSprite();
	}
	else if (!IsAlive() && !IsSpriteAnimationFinished())
	{
		VFX::SpawnBloodFountain(GetPosition(), particleManager);
	}
}

//all enemies will have flipped sprite, i should flip it on the rendering and i need each enemy to have variable that corresponds to that
void Enemy::UpdateSprite()
{
	if (m_State == EnemyState::turn)
	{
		float
			positionX{ GetPositionX() };

		if (m_CurrentTargetControlPoint == -1)
		{
			return;
		}

		//if turn is toggled, it means that new direction is opposite direction from previous direction, so we can just check in a opposite way
		// (not controlPointposX < positionX but controlPointposX > positionX
		if ((*m_ControlPoints)[m_CurrentTargetControlPoint].position.x > positionX)
		{
			GetSprite()->ResetHorizontalFlip();
		}
		else
		{
			GetSprite()->FlipHorizontally();
		}
		return;
	}

	if (GetVelocityX() > 0)
	{
		GetSprite()->ResetHorizontalFlip();
	}
	else if (GetVelocityX() < 0)
	{
		GetSprite()->FlipHorizontally();
	}
}

void Enemy::SetState(EnemyState state)
{
	if (state == m_State) { return; }

	m_State = state;
	
	StateInitialization(m_State);
	
	GetSprite()->SetAnimationFrameInfo((*m_EnemySpriteFrames)[static_cast<int>(m_State)]);
	ResetAnimation();
}

void Enemy::Kill(const Vector2f& impulse)
{
	Entity::Kill(impulse);
	SetState(EnemyState::dead);
}

const Entity* Enemy::GetTarget()
{
	return m_pTarget;
}

void Enemy::UpdateCurrentState(float elapsedSec, ParticleManager* particleManager, const Map* pMap )
{
	switch (m_State)
	{
	case EnemyState::idle:
	{
		UpdateIdle(elapsedSec, pMap);
		break;
	}
	case EnemyState::walk:
	{
		UpdateWalk(elapsedSec, pMap);
		break;
	}
	case EnemyState::run:
	{
		UpdateRun(elapsedSec);
		break;
	}
	case EnemyState::attack:
	{
		UpdateAttack(elapsedSec, particleManager);
		break;
	}
	case EnemyState::turn:
	{
		UpdateTurn(elapsedSec);
		break;
	}
	case EnemyState::dead:
	{
		break;
	}

	}
}

void Enemy::UpdateIdle(float elapsedSec, const Map* pMap)
{
	if (m_pTarget != nullptr)
	{
		if (CanSeeTarget(pMap))
		{
			if (IsTargetInAttackRange())
			{
				SetState(EnemyState::attack);
			}
			else
			{
				SetState(EnemyState::run);
			}
		}
	}
}

void Enemy::UpdateWalk(float elapsedSec, const Map* pMap)
{
	Patrol(elapsedSec);

	if (m_pTarget != nullptr)
	{
		if (CanSeeTarget(pMap))
		{
			SetState(EnemyState::run);
		}
	}
	
}

void Enemy::UpdateRun(float elapsedSec)
{
	Chase(elapsedSec);
}

void Enemy::UpdateAttack(float elapsedSec, ParticleManager* particleManager) 
{
	const int
		attackTriggerFrame{ 3};
	if (GetSprite()->GetCurrentFrameCount() == attackTriggerFrame)
	{
		Attack(particleManager);
	}
	if (IsSpriteAnimationFinished())
	{
		SetState(EnemyState::run);
	}
}

void Enemy::UpdateTurn(float elapsedSec)
{
	SetVelocityX(0.f);
	if (IsSpriteAnimationFinished())
	{
		SetState(EnemyState::walk); //have stored previous state or calculate depending on player position
	}
}

void Enemy::StateInitialization(EnemyState state)
{
	switch (state)
	{
	case EnemyState::idle:
	{
		SetVelocityX(0.f);
		break;
	}
	case EnemyState::attack:
	{
		SetVelocityX(0.f);
	}
	case EnemyState::run:
	{
		m_Path.clear();
		break;
	}
	}
}

void Enemy::Patrol(float elapsedSec)
{
	const float
		deltaEps{ 1.f };

	if ((*m_ControlPoints).empty())
	{
		return;
	}

	if ((*m_ControlPoints)[m_CurrentTargetControlPoint].type == ControlPoint::ControlPointType::stairSignifier)
	{
		m_CurrentTargetControlPoint = PathFinder::GetNextControlPointIdxByType(
			m_CurrentTargetControlPoint,
			false,
			GetFloor(),
			ControlPoint::ControlPointType::leadingPoint,
			(*m_ControlPoints)
		);
	}

	if (MoveTo((*m_ControlPoints)[m_CurrentTargetControlPoint], m_PatrolSpeedMultiplier))
	{
		int nextTargetIndex{ PathFinder::GetNextControlPointIdxByType(
			m_CurrentTargetControlPoint,
			false,
			GetFloor(),
			ControlPoint::ControlPointType::leadingPoint,
			(*m_ControlPoints)
		) };

		if (nextTargetIndex == -1)
		{
			return;
		}

		float deltaXToNext{
			(*m_ControlPoints)[nextTargetIndex].position.x - GetPositionX()
		};

		m_CurrentTargetControlPoint = nextTargetIndex;
		
		if (std::abs(deltaXToNext) > deltaEps)
		{
			int nextDirection{ 1 };
			if (deltaXToNext < 0.f)
			{
				nextDirection = -1;
			}

			if (nextDirection != GetFacingDirection())
			{
				SetState(EnemyState::turn);
			}
		}
	}
}

void Enemy::Chase(float elapsedSec)
{
	int floor{ GetFloor() };

	if (m_pTarget == nullptr)
	{
		SetState(EnemyState::walk);
		return;
	}

	if (m_pTarget->GetFloor() != floor)
	{
		if (m_Path.empty())
		{
			m_Path = PathFinder::CalculatePathToFloor(
				m_CurrentTargetControlPoint,
				m_pTarget->GetFloor(),
				GetFloor(),
				(*m_ControlPoints)
			);
		}

		if (!m_Path.empty())
		{
			int 
				targetIndex{ m_Path.back() };

			if (MoveTo((*m_ControlPoints)[targetIndex], m_RunningMultiplier))
			{
				if ((*m_ControlPoints)[targetIndex].type == ControlPoint::ControlPointType::stairSignifier)
				{
					SetCanJumpThroughPlatform(!CanJumpThroughPlatform());//flip canjump state
						//TODO look more into it because it can cause issues with enemy movement

					SetFloor((*m_ControlPoints)[targetIndex].floor);

					int floor{ GetFloor() };

					m_Path.pop_back();
				}
			}

			return;
		}
	}

	if (!m_Path.empty())
	{
		m_Path.clear();
	}

	ControlPoint playerPoint{
		m_pTarget->GetPosition(),
		ControlPoint::ControlPointType::none,
		GetFloor()
	};

	MoveTo(playerPoint, m_RunningMultiplier);

	if (IsTargetInAttackRange() && m_AttackCooldownTimer <= 0 && m_pTarget->IsAlive())
	{
		SetState(EnemyState::attack);
	}
}

bool Enemy::MoveTo(const ControlPoint& controlPoint, float speedMultiplier)
{
	//TODO check if different floors, if so i can search for path
	//maybe i can move my path finding from chase to here
	
	const float
		reachDistance{ 10.f };

	float 
		deltaX{ controlPoint.position.x - GetPositionX() };

	if (std::abs(deltaX) < reachDistance)
	{
		return true;
	}

	else
	{
		int direction{ 1 };
		if (deltaX < 0)
		{
			direction = -1;
		}

		SetVelocityX(GetSpeed() * speedMultiplier * direction);
		return false;
	}
}


void Enemy::ResetAnimation() const
{
	GetSprite()->ResetAnimation();
}

bool Enemy::CanSeeTarget(const Map* pMap)
{
	if (m_pTarget != nullptr)
	{
		const Vector2f
			targetPosition{ m_pTarget->GetPosition() };

		float
			distance{ utils::GetDistance(GetPosition(), targetPosition) };

		if (distance > m_DetectionRange)
		{
			return false;
		}

		float
			directionToPlayer{ targetPosition.x - GetPositionX() };

		bool
			isPlayerToRight{ directionToPlayer > 0 },
			isLookingRight{ GetFacingDirection() == 1 };

		Vector2f
			positionCorrection{ 0.f, 5.f };

		return (isPlayerToRight == isLookingRight) &&
			(m_pTarget->GetFloor() == GetFloor()) && 
			!pMap->AreSeparatedByActiveObject(GetPosition() + positionCorrection, targetPosition + positionCorrection);
		//TODO apply position correction on check like this in more robust way
	}
	else
	{
		return false;
	}
}

bool Enemy::IsTargetInAttackRange()
{
	if (m_pTarget != nullptr)
	{
		float
			distance{ utils::GetDistance(GetPosition(), m_pTarget->GetPosition()) };

		return (distance <= m_AttackRange) &&
			(m_pTarget->GetFloor() == GetFloor());
	}
	else
	{
		return false;
	}
}

Enemy::EnemyState Enemy::GetState() const
{
	return m_State;
}

float Enemy::GetAttackCooldown() const
{
	return m_AttackCooldownTimer;
}

void Enemy::ResetAttackCooldown() 
{
	m_AttackCooldownTimer = m_AttackCooldown;
}

void Enemy::UpdateCooldowns(float elapsedSec)
{
	m_AttackCooldownTimer -= elapsedSec;
	if (m_AttackCooldownTimer < 0)
	{
		m_AttackCooldownTimer = 0.f;
	}
}
