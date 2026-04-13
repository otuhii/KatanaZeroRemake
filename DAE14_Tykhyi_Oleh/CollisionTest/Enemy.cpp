#include "pch.h"
#include "Enemy.h"

#include "PathFinder.h"

#include "utils.h"

Enemy::Enemy(
	EnemyType type, 
	EnemyState state,
	Sprite* pSprite, 
	const std::vector<AnimationFrameInfo>* enemyAnimationFrames,
	const Vector2f& position,
	float speed, 
	float scale, 
	int floor, 
	float playerDetectionRange, 
	float attackRange)
	:  
	Entity(pSprite, position, Vector2f{0.f, 0.f}, speed, floor),
	m_EnemySpriteFrames{ enemyAnimationFrames },
	m_State{state},
	m_Type{type},
	m_AttackRange{attackRange},
	m_DetectionRange{playerDetectionRange}
{
	GetSprite()->SetAnimationFrameInfo((*m_EnemySpriteFrames)[static_cast<int>(m_State)]);
	GetSprite()->SetScale(scale);

	UpdateHitbox();
}

void Enemy::Draw() const
{
	Entity::Draw();

	utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	utils::DrawRect(GetHitbox());
}

void Enemy::UpdateControlPoints(const std::vector<ControlPoint>* controlPoints)
{
	m_ControlPoints = controlPoints;
}


Enemy::EnemyType Enemy::GetType() const
{
	return m_Type;
}

void Enemy::Update(float elapsedSec, const Vector2f& playerPos, int playerFloor, ParticleManager* particleManager, const Rectf& viewport)
{
	UpdateCooldowns(elapsedSec);
	Entity::Update(elapsedSec, viewport);
	UpdateCurrentState(elapsedSec, playerPos, playerFloor, particleManager);

	UpdateSprite();
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
	GetSprite()->ResetAnimation();
}


void Enemy::UpdateCurrentState(float elapsedSec, const Vector2f& playerPos, int playerFloor, ParticleManager* particleManager)
{
	switch (m_State)
	{
	case EnemyState::idle:
	{
		UpdateIdle(elapsedSec, playerPos, playerFloor);
		break;
	}
	case EnemyState::walk:
	{
		UpdateWalk(elapsedSec, playerPos, playerFloor);
		break;
	}
	case EnemyState::run:
	{
		UpdateRun(elapsedSec, playerPos, playerFloor);
		break;
	}
	case EnemyState::attack:
	{
		UpdateAttack(elapsedSec, playerPos, particleManager);
		break;
	}
	case EnemyState::turn:
	{
		UpdateTurn(elapsedSec);
		break;
	}

	}
}

void Enemy::UpdateIdle(float elapsedSec, const Vector2f& playerPos, int playerFloor)
{
	if (CanSeePlayer(playerPos, playerFloor))
	{
		if (IsPlayerInAttackRange(playerPos))
		{
			SetState(EnemyState::attack);
		}
	}
}

void Enemy::UpdateWalk(float elapsedSec, const Vector2f& playerPos, int playerFloor)
{
	Patrol(elapsedSec);

	if (CanSeePlayer(playerPos, playerFloor))
	{
		SetState(EnemyState::run);
	}
}

void Enemy::UpdateRun(float elapsedSec, const Vector2f& playerPos, int playerFloor)
{
	Chase(elapsedSec, playerPos, playerFloor);
}

void Enemy::UpdateAttack(float elapsedSec, const Vector2f& playerPos, ParticleManager* particleManager) 
{
	const int
		attackTriggerFrame{ 3};
	if (GetSprite()->GetCurrentFrameCount() == attackTriggerFrame)
	{
		Attack(playerPos, particleManager);
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
		m_AttackCooldownTimer = m_AttackCooldown;
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
			GetFloor(),
			ControlPoint::ControlPointType::leadingPoint,
			(*m_ControlPoints)
		);
	}

	if (MoveTo((*m_ControlPoints)[m_CurrentTargetControlPoint], m_PatrolSpeedMultiplier))
	{
		int nextTargetIndex{ PathFinder::GetNextControlPointIdxByType(
			m_CurrentTargetControlPoint,
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

void Enemy::Chase(float elapsedSec, const Vector2f& playerPos, int playerFloor)
{
	int floor{ GetFloor() };

	if (playerFloor != floor)
	{
		if (m_Path.empty())
		{
			m_Path = PathFinder::CalculatePathToFloor(
				m_CurrentTargetControlPoint,
				playerFloor,
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
		playerPos,
		ControlPoint::ControlPointType::none,
		GetFloor()
	};

	MoveTo(playerPoint, m_RunningMultiplier);

	if (IsPlayerInAttackRange(playerPos) && m_AttackCooldownTimer <= 0)
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

bool Enemy::CanSeePlayer(const Vector2f& playerPos, int playerFloor)
{
	float
		distance{ utils::GetDistance(GetPosition(), playerPos) };

	if (distance > m_DetectionRange)
	{
		return false;
	}

	float
		directionToPlayer{ playerPos.x - GetPositionX() };

	bool
		isPlayerToRight{ directionToPlayer > 0 },
		isLookingRight{ GetFacingDirection() == 1 };

	return (isPlayerToRight == isLookingRight) &&
			(playerFloor == GetFloor());
}

bool Enemy::IsPlayerInAttackRange(const Vector2f& playerPos)
{
	float
		distance{ utils::GetDistance(GetPosition(), playerPos) };

	return distance <= m_AttackRange;
}

Enemy::EnemyState Enemy::GetState() const
{
	return m_State;
}

float Enemy::GetAttackCooldownTimer() const
{
	return m_AttackCooldownTimer;
}

void Enemy::ResetAttackCooldownTimer() 
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
