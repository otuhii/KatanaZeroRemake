#include "pch.h"
#include "Enemy.h"

#include "utils.h"

Enemy::Enemy(EnemyType type, Sprite* sprite, const std::vector<AnimationFrameInfo>& enemyAnimationFrames, const Vector2f& position, float speed, float scale)
	:  Entity(sprite, position, Vector2f{0.f, 0.f}, speed),
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
	//utils::DrawRect(GetHitbox());
}

void Enemy::UpdatePath(const std::vector<Vector2f>& controlPoints)
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
	}
	case EnemyState::run:
	{
		//Chase(elapsedSec, playerPos);
		break;
	}
	}
}


void Enemy::PlayerSensing(const Vector2f& playerPos)
{
	const float
		triggerDistance{ 200.f },
		runningSpeedMultiplier{ 1.5f };

	float
		distance{ (playerPos - GetPosition()).Length() };

	if (distance < triggerDistance)
	{
		SetState(EnemyState::run);
		float 
			direction{ 1.f };
		if (playerPos.x < GetPositionX())
		{
			direction = -1;
		}
		SetVelocityX(GetSpeed() * runningSpeedMultiplier * direction);
	}
	else if (m_State == EnemyState::run)
	{
		SetState(EnemyState::walk);
	}
}


void Enemy::Patrol(float elapsedSec)
{
	if (m_Path.empty())
	{
		return;
	}

	Vector2f
		target{ m_Path[m_CurrentTargetControlPoint] };

	float
		reachMinDistance{ 5.f };

	float
		deltaX{ target.x - GetPositionX() };

	if (std::abs(deltaX) < reachMinDistance)
	{
		m_CurrentTargetControlPoint = (m_CurrentTargetControlPoint + 1) % m_Path.size();
		SetVelocityX(0.f);
	}
	else
	{
		float direction{ 1.f };
		if (deltaX < 0)
		{
			direction = -1.f;
		}
		float velocityX{ GetSpeed() * direction };

		SetVelocityX(velocityX);
	}
}

void Enemy::Chase(float elapsedSec, const Vector2f& playerPos)
{

}
