#include "pch.h"
#include "Enemy.h"


Enemy::Enemy(EnemyType type, Sprite* sprite, const std::vector<AnimationFrameInfo>& enemyAnimationFrames, const Vector2f& position, float speed, float scale)
	:  Entity(sprite, position, Vector2f{0.f, 0.f}, speed),
	m_EnemySpriteFrames{ enemyAnimationFrames },
	m_State{EnemyState::idle},
	m_Type{type}
{
	GetSprite()->SetAnimationFrameInfo(m_EnemySpriteFrames[static_cast<int>(m_State)]);
	GetSprite()->SetScale(scale);

	UpdateHitbox();
}

void Enemy::Draw() const
{
	Entity::Draw();
}

void Enemy::Update(float elapsedSec, const Rectf& viewport)
{
	Entity::Update(elapsedSec, viewport);
}

void Enemy::SetState(EnemyState state)
{
}
