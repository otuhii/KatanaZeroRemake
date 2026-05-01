#include "pch.h"
#include "Grunt.h"

#include "ParticleManager.h"

Grunt::Grunt(
	Sprite* pSprite,
	Sprite* pSlashSprite,
	const Entity* pTarget,
	const std::vector<AnimationFrameInfo>* enemyAnimationFrames,
	const std::vector<AnimationFrameInfo>* enemyProjectileAnimationFrames,
	const Vector2f& position,
	float speed,
	float scale,
	int floor,
	float playerDetectionRange,
	float attackRange)
	:Enemy{
		EnemyType::grunt,
		EnemyState::walk,
		pSprite,
		pTarget,
		enemyAnimationFrames,
		position,
		speed,
		scale,
		floor,
		playerDetectionRange,
		attackRange
	},
	m_pSlashSprite{pSlashSprite}
{
	m_pSlashSprite->SetScale(GetSprite()->GetScale());
	m_pSlashSprite->SetAnimationFrameInfo((*enemyProjectileAnimationFrames)[0]);
}

void Grunt::Draw() const
{
	Enemy::Draw();

	if (GetState() == EnemyState::attack)
	{
		m_pSlashSprite->Draw(GetPosition(), true, false);
	}
}

void Grunt::Attack(ParticleManager* particleManager)
{
	if (GetAttackCooldown() <= 0.f)
	{
		ResetAttackCooldown();
	}
	SpawnAttackParticle(particleManager);
}


void Grunt::SpawnAttackParticle(ParticleManager* particleManager) const
{
	Rectf
		gruntDims{ GetCurrentHitbox()};

	Vector2f
		positionOffset{ 0.f, gruntDims.height};

	particleManager->SpawnMelee(
		AttackParticle::OwnerType::Enemy,
		this,
		GetPosition(),
		positionOffset,
		m_BaseAttackHitbox,
		0.2f,
		0.f,
		GetSprite()->IsFlippedHorizontally(),
		GetSprite()->IsFlippedVertically()
	);
}

void Grunt::ResetAnimation() const
{
	Enemy::ResetAnimation();
	m_pSlashSprite->ResetAnimation();

	if (GetSprite()->IsFlippedHorizontally())
	{
		m_pSlashSprite->FlipHorizontally();
	}
	else
	{
		m_pSlashSprite->ResetHorizontalFlip();
	}

}
