#include "pch.h"
#include "Enemy.h"

#include "utils.h"

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
	m_ControlPoints = controlPoints;
}

void Enemy::Update(float elapsedSec, const Vector2f& playerPos, int playerFloor, const Rectf& viewport)
{
	Entity::Update(elapsedSec, viewport);
	PlayerSensing(playerPos);
	HandleCurrentState(elapsedSec, playerPos, playerFloor);

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

void Enemy::HandleCurrentState(float elapsedSec, const Vector2f& playerPos, int playerFloor)
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
		HandleChase(elapsedSec, playerPos, playerFloor);
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
	if (m_ControlPoints.empty())
	{
		return;
	}

	if (MoveTo(m_ControlPoints[m_CurrentTargetControlPoint], 1.f))
	{
		m_CurrentTargetControlPoint = FindNextLeadingPoint();
	}
}

void Enemy::HandleChase(float elapsedSec, const Vector2f& playerPos, int playerFloor)
{
	const float
		runningMultiplier{ 1.5f };

	const int floor{
		GetFloor() 
	};

	if (playerFloor != floor)
	{
		if (m_Path.empty())
		{
			m_Path = CalculatePath(playerFloor);
		}

		if (!m_Path.empty())
		{
			int targetIdx{ m_Path.back() };

			if (MoveTo(m_ControlPoints[targetIdx], runningMultiplier))
			{
				if (m_ControlPoints[targetIdx].type == ControlPoint::ControlPointType::stairSignifier &&
					CanJumpThroughPlatform())
				{
					SetCanJumpThroughPlatform(false);
				}
				else if (m_ControlPoints[targetIdx].type == ControlPoint::ControlPointType::stairSignifier &&
					!CanJumpThroughPlatform())
				{
					SetCanJumpThroughPlatform(true);
				}

				SetFloor(m_ControlPoints[targetIdx].floor);
				m_CurrentTargetControlPoint = targetIdx;

				m_Path.pop_back();
			}
			return;
		}
	}
	

	if (!m_Path.empty())
	{
		m_Path.clear();
	}

	ChasePlayer(elapsedSec, playerPos);
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
	size_t startIndex = (m_CurrentTargetControlPoint + 1) % m_ControlPoints.size();

	for (size_t index{ 0 }; index < m_ControlPoints.size(); ++index)
	{
		size_t searchIndex{ (startIndex + index) % m_ControlPoints.size() };

		if (m_ControlPoints[searchIndex].floor == GetFloor() &&
			m_ControlPoints[searchIndex].type == ControlPoint::ControlPointType::leadingPoint)
		{
			return static_cast<int>(searchIndex);
		}
	}

	return -1;
}

int Enemy::GetIndexByPointId(int id)
{
	//i could use map but fuck that, not that much points for O(n)

	for (size_t index{ 0 }; index < m_ControlPoints.size(); ++index)
	{
		if (m_ControlPoints[index].pointId == id)
		{
			return static_cast<int>(index);
		}
	}

	return -1;
}

//BFS search for path to target floor
std::vector<int> Enemy::CalculatePath(int targetFloor)
{
	if (m_ControlPoints.empty())
	{
		return {};
	}

	std::vector<int> parentIndices(m_ControlPoints.size(), -1);
	std::vector<bool> visited(m_ControlPoints.size(), false);

	std::vector<int> frontier;
	int head{ 0 };

	int startIdx{ GetClosestControlPointIdx() };
	frontier.push_back(startIdx);
	visited[startIdx] = true;

	int reachedTargetIdx{ -1 };

	while (head < static_cast<int>(frontier.size()))
	{
		int currentIdx{ frontier[head] };

		head++;

		if (m_ControlPoints[currentIdx].floor == targetFloor)
		{
			reachedTargetIdx = currentIdx;
			break;
		}

		for (int neightborId : m_ControlPoints[currentIdx].neightborIndices)
		{
			int neighborIdx{ GetIndexByPointId(neightborId) };

			if (neighborIdx != -1 && !visited[neighborIdx])
			{
				visited[neighborIdx] = true;
				parentIndices[neighborIdx] = currentIdx;
				frontier.push_back(neighborIdx);
			}
		
		}
	}

	std::vector<int> path;

	if (reachedTargetIdx != -1)
	{
		int currentIdx{ reachedTargetIdx };

		while ( currentIdx != -1)
		{
			path.push_back(currentIdx);

			if (currentIdx == startIdx)
			{
				break;
			}

			currentIdx = parentIndices[currentIdx];
		}
	}

	return path;
}

int Enemy::FindStairs(int floor)
{
	size_t startIndex{ (m_CurrentTargetControlPoint + 1) % m_ControlPoints.size() };

	for (size_t index{ 0 }; index < m_ControlPoints.size(); ++index)
	{
		size_t searchIndex{ (startIndex + index) % m_ControlPoints.size() };

		if (m_ControlPoints[searchIndex].floor == GetFloor() &&
			m_ControlPoints[searchIndex].type == ControlPoint::ControlPointType::stairSignifier)
		{
			return static_cast<int>(searchIndex);
		}
	}


	return -1;
}

int Enemy::GetClosestControlPointIdx() const
{
	int closestIdx{ 0 };
	float minDistance{ 999999.f };

	for (int i{ 0 }; i < static_cast<int>(m_ControlPoints.size()); ++i)
	{
		if (m_ControlPoints[i].floor == GetFloor() && m_ControlPoints[i].type == ControlPoint::ControlPointType::stairSignifier)
		{
			float dist{ std::abs(m_ControlPoints[i].position.x - GetPositionX()) };
			if (dist < minDistance)
			{
				minDistance = dist;
				closestIdx = i;
			}
		}
	}
	return closestIdx;
}