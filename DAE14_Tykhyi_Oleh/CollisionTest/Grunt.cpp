#include "pch.h"
#include "Grunt.h"

#include "ParticleManager.h"

Grunt::Grunt(
	Sprite* pSprite,
	const Entity* pTarget,
	const std::vector<AnimationFrameInfo>* enemyAnimationFrames,
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
	}
{
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
