#include "pch.h"
#include "Grunt.h"

#include "ParticleManager.h"

Grunt::Grunt(
	Sprite* pSprite,
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

void Grunt::Attack(const Vector2f& playerPos, ParticleManager* particleManager)
{
	SpawnAttackParticle(particleManager);
}

void Grunt::SpawnAttackParticle(ParticleManager* particleManager) const
{
	Rectf
		gruntDims{ GetHitbox()};

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
