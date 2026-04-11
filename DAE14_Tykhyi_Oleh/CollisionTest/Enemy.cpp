#include "pch.h"
#include "Enemy.h"

#include "utils.h"

#include <iostream>

Enemy::Enemy(EnemyType type, Sprite* sprite, const std::vector<AnimationFrameInfo>& enemyAnimationFrames, const Vector2f& position, float speed, float scale, int floor)
	:  Entity(sprite, position, Vector2f{0.f, 0.f}, speed, floor),
	m_EnemySpriteFrames{ enemyAnimationFrames },
	m_State{EnemyState::walk},
	m_Type{type}
{
	GetSprite()->SetAnimationFrameInfo(m_EnemySpriteFrames[static_cast<int>(m_State)]);
	GetSprite()->SetScale(scale);

	UpdateHitbox();
}

void Enemy::Draw() const
{
	Entity::Draw();

	utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	utils::DrawRect(GetHitbox());
}

void Enemy::UpdatePath(const std::vector<ControlPoint>& controlPoints)
{
	m_Path = controlPoints;
}

void Enemy::Update(float elapsedSec, const Vector2f& playerPos, const Rectf& viewport)
{
	Entity::Update(elapsedSec, viewport);
	PlayerSensing(playerPos);
	HandleCurrentState(elapsedSec, playerPos);

	UpdateSprite();
}

void Enemy::UpdateSprite()
{
	//all enemies will have flipped sprite, i should flip it on the rendering and i need each enemy to have variable that corresponds to that
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

	GetSprite()->SetAnimationFrameInfo(m_EnemySpriteFrames[static_cast<int>(m_State)]);
	GetSprite()->ResetAnimation();
}

void Enemy::HandleCurrentState(float elapsedSec, const Vector2f& playerPos)
{
	switch (m_State)
	{
	case EnemyState::walk:
	{
		Patrol(elapsedSec);
		break;
	}
	case EnemyState::idle:
	{
		SetVelocity(Vector2f{ 0.f, 0.f });
		break;
	}
	case EnemyState::run:
	{
		HandleChase(elapsedSec, playerPos);
		break;
	}
	}
}

void Enemy::PlayerSensing(const Vector2f& playerPos)
{
	const float
		triggerDistance{ 500.f };

	float
		distance{ (playerPos - GetPosition()).Length() };

	if (distance < triggerDistance)
	{
		SetState(EnemyState::run);
	}
}

void Enemy::Patrol(float elapsedSec)
{
	if (m_Path.empty())
	{
		return;
	}

	if (MoveTo(m_Path[m_CurrentTargetControlPoint], 1.f))
	{
		m_CurrentTargetControlPoint = FindNextLeadingPoint();
	}
}

void Enemy::HandleChase(float elapsedSec, const Vector2f& playerPos)
{
	const float
		floorHeightThreshold{ 200.f };

	const float
		runningMultiplier{ 1.5f };

	float
		deltaX{ playerPos.x - GetPositionX() },
		deltaY{ playerPos.y - GetPositionY() };

	if (deltaY < -floorHeightThreshold)
	{
		if (CanJumpThroughPlatform())
		{
			ChasePlayer(elapsedSec, playerPos);
		}
		else
		{
			int stairIdx{ FindStairs(GetFloor())};
			if (stairIdx != -1)
			{
				if (MoveTo(m_Path[stairIdx], runningMultiplier))
				{
					SetCanJumpThroughPlatform(true);
					ChasePlayer(elapsedSec, playerPos);
					int floor{ GetFloor() }; //because player is on the previous floor
					SetFloor(floor--);
				}
				return;
			}
		}
	}
	else if (deltaY > floorHeightThreshold)
	{
		if (CanJumpThroughPlatform())
		{
			ChasePlayer(elapsedSec, playerPos);
		}
		else
		{
			int stairIdx{ FindStairs(GetFloor()) };
			if (stairIdx != -1)
			{
				if (MoveTo(m_Path[stairIdx], runningMultiplier))
				{
					SetCanJumpThroughPlatform(true);
					ChasePlayer(elapsedSec, playerPos);
					int floor{ GetFloor() }; // becasue player is on the next floor
					SetFloor(floor++);
				}
				return;
			}
		}
	}
	else
	{
		SetCanJumpThroughPlatform(false);
		ChasePlayer(elapsedSec, playerPos);
	}
}

void Enemy::ChasePlayer(float elapsedSec, const Vector2f& playerPos)
{
	const float
		runningMultiplier{ 1.5f };

	ControlPoint playerPoint{
		playerPos,
		ControlPoint::ControlPointType::none,
		GetFloor()
	};

	MoveTo(playerPoint, runningMultiplier);
}

bool Enemy::MoveTo(const ControlPoint& controlPoint, float speedMultiplier)
{
	const float
		reachMinDistance{ 10.f };

	float
		deltaX{ controlPoint.position.x - GetPositionX() };

	if (std::abs(deltaX) < reachMinDistance)
	{
		SetVelocityX(0.f);

		return true;
	}
	else
	{
		float direction{ 1.f };
		if (deltaX < 0)
		{
			direction = -1;
		}

		SetVelocityX(GetSpeed() * speedMultiplier * direction);
		return false;
	}
}

int Enemy::FindNextLeadingPoint()
{
	size_t startIndex = (m_CurrentTargetControlPoint + 1) % m_Path.size();

	for (size_t index{ 0 }; index < m_Path.size(); ++index)
	{
		size_t searchIndex{ (startIndex + index) % m_Path.size() };

		if (m_Path[searchIndex].floor == GetFloor() &&
			m_Path[searchIndex].type == ControlPoint::ControlPointType::leadingPoint)
		{
			return static_cast<int>(searchIndex);
		}
	}

	return -1;
}

int Enemy::FindStairs(int floor)
{
	size_t startIndex{ (m_CurrentTargetControlPoint + 1) % m_Path.size() };

	for (size_t index{ 0 }; index < m_Path.size(); ++index)
	{
		size_t searchIndex{ (startIndex + index) % m_Path.size() };

		if (m_Path[searchIndex].floor == GetFloor() &&
			m_Path[searchIndex].type == ControlPoint::ControlPointType::stairSignifier)
		{
			return static_cast<int>(searchIndex);
		}
	}


	return -1;
}