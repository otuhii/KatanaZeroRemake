#include "pch.h"
#include "Gangster.h"

#include "ParticleManager.h"
#include "SoundManager.h"

Gangster::Gangster(
	Sprite* pSprite,
	Sprite* pGunSprite,
	Sprite* pBulletProjectile,
	const Entity* pTarget,
	const SoundManager* pSoundManager,
	const std::vector<AnimationFrameInfo>* enemyAnimationFrames, 
	const Vector2f& position,
	float speed, 
	float scale, 
	int floor, 
	float playerDetectionRange,
	float attackRange)
	:Enemy{
		EnemyType::gangster,
		EnemyState::idle,
		pSprite,
		pTarget,
		pSoundManager,
		enemyAnimationFrames,
		position,
		speed,
		scale,
		floor,
		playerDetectionRange,
		attackRange
	},
	m_pGunSprite{pGunSprite},
	m_pBulletProjectileSprite{pBulletProjectile}
{
	m_pGunSprite->SetScale(GetSprite()->GetScale());

}

void Gangster::Draw() const
{
	Enemy::Draw();
	
	if (GetState() == EnemyState::attack)
	{
		Vector2f gunPosition{ GetPosition() };
		gunPosition.y += GetCurrentHitbox().height * 0.6f;

		m_pGunSprite->Draw(gunPosition, true, true);
	}
	
}

void Gangster::Update(float elapsedSec, ParticleManager* particleManager, const Map* pMap, const Rectf& viewport)
{
	if (!GetTarget()->IsAlive())
	{
		SetState(EnemyState::idle);
	}
	Enemy::Update(elapsedSec, particleManager, pMap, viewport);
}


void Gangster::Attack(ParticleManager* particleManager)
{
	if (m_AimTimer <= 0.f)
	{
		Shoot(particleManager);
	}
}


void Gangster::UpdateCurrentState(float elapsedSec, ParticleManager* particleManager, const Map* pMap)
{
	switch (GetState())
	{
	case EnemyState::idle:
	{
		UpdateIdle(elapsedSec, pMap);
		break;
	}
	case EnemyState::run:
	{
		UpdateRun(elapsedSec);
		break;
	}
	case EnemyState::attack:
		UpdateAttack(elapsedSec, particleManager);
		if (!IsTargetInAttackRange())
		{
			SetState(EnemyState::run);
		}
		break;
	case EnemyState::dead:
		break;
	}
}

void Gangster::UpdateAttack(float elapsedSec, ParticleManager* particleManager)
{
	Aim();
	if (GetAttackCooldown() <= 0.f)
	{
		UpdateAimTime(elapsedSec);
		Attack(particleManager);
	}
}

void Gangster::UpdateGunSprite()
{
	if (GetSprite()->IsFlippedHorizontally())
	{
		m_pGunSprite->FlipHorizontally();
	} 
	else 
	{
		m_pGunSprite->ResetHorizontalFlip();
	}
}

void Gangster::Aim()
{
	Vector2f
		currentPosition{ GetPosition() },
		targetPosition{ GetTarget()->GetPosition() };

	currentPosition.x += GetCurrentHitbox().width * 0.5f;
	currentPosition.y += GetCurrentHitbox().height * 0.5f;
	
	targetPosition.y += GetTarget()->GetCurrentHitbox().height * 0.5f;

	Vector2f
		direction{ targetPosition - currentPosition};

	float
		rotationAngle{ (180 / static_cast<float>(M_PI)) * std::atan2(direction.y, direction.x) };


	if (rotationAngle > 90 || rotationAngle < -90)
	{
		rotationAngle -= 180.f;
		m_pGunSprite->FlipHorizontally();
		GetSprite()->FlipHorizontally();
	}
	else
	{
		m_pGunSprite->ResetHorizontalFlip();
		GetSprite()->ResetHorizontalFlip();
	}


	m_pGunSprite->RotateBy(rotationAngle);
}

void Gangster::UpdateAimTime(float elapsedSec)
{
	m_AimTimer -= elapsedSec;
	if (m_AimTimer < 0.f)
	{
		m_AimTimer = 0.f;
	}
}

void Gangster::Shoot(ParticleManager* particleManager)
{
	GetSoundManager()->Play(SoundManager::SoundEffectType::gunFire, 0);

	float angleRad{
		m_pGunSprite->GetRotation() * (static_cast<float>(M_PI) / 180.f)
	};

	Vector2f bulletVelocity{
		std::cosf(angleRad) * bulletSpeed * GetFacingDirection(),
		std::sinf(angleRad) * bulletSpeed * GetFacingDirection()
	};

	particleManager->SpawnBullet(
		AttackParticle::OwnerType::Enemy,
		GetPosition(),
		Vector2f{ 0, GetCurrentHitbox().height * 0.6f },
		m_BulletHitbox,
		bulletVelocity,
		m_pGunSprite->GetRotation(),
		m_pGunSprite->IsFlippedHorizontally(),
		false,
		m_pBulletProjectileSprite
	);

	ResetAttackCooldown();

	m_AimTimer = m_AimTime;
}

