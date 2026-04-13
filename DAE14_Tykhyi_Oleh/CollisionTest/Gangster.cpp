#include "pch.h"
#include "Gangster.h"

#include "ParticleManager.h"

Gangster::Gangster(
	Sprite* pSprite,
	Sprite* pGunSprite,
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
		enemyAnimationFrames,
		position,
		speed,
		scale,
		floor,
		playerDetectionRange,
		attackRange
	}
{
	m_GunSprite = pGunSprite;
	m_GunSprite->SetScale(GetSprite()->GetScale());
}

void Gangster::Draw() const
{
	Enemy::Draw();
	
	if (GetState() == EnemyState::attack)
	{
		Vector2f gunPosition{ GetPosition() };
		gunPosition.y += GetHitbox().height * 0.6f;

		m_GunSprite->Draw(gunPosition, true, true);
	}
	
}

void Gangster::Update(float elapsedSec, const Vector2f& playerPos, int playerFloor, ParticleManager* particleManager, const Rectf& viewport)
{
	Enemy::Update(elapsedSec, playerPos, playerFloor, particleManager, viewport);
}


void Gangster::Attack(const Vector2f& playerPos, ParticleManager* particleManager)
{
	if (m_AimTimer <= 0.f)
	{
		Shoot(particleManager);
	}
}

void Gangster::UpdateCurrentState(float elapsedSec, const Vector2f& playerPos, int playerFloor, ParticleManager* particleManager)
{
	switch (GetState())
	{
	case EnemyState::idle:
	{
		UpdateIdle(elapsedSec, playerPos, playerFloor);
		break;
	}
	case EnemyState::run:
	{
		UpdateRun(elapsedSec, playerPos, playerFloor);
		break;
	}
	case EnemyState::attack:
		UpdateAttack(elapsedSec, playerPos, particleManager);
		if (!IsPlayerInAttackRange(playerPos, playerFloor))
		{
			SetState(EnemyState::run);
		}
		break;
	}
}

void Gangster::UpdateAttack(float elapsedSec, const Vector2f& playerPos, ParticleManager* particleManager)
{
	Aim(playerPos);
	if (GetAttackCooldown() <= 0.f)
	{
		UpdateAimTime(elapsedSec);
		Attack(playerPos, particleManager);
	}
}

void Gangster::UpdateGunSprite()
{
	if (GetSprite()->IsFlippedHorizontally())
	{
		m_GunSprite->FlipHorizontally();
	} 
	else 
	{
		m_GunSprite->ResetHorizontalFlip();
	}
}

void Gangster::Aim(const Vector2f& playerPos)
{
	Vector2f
		currentPosition{ GetPosition() };

	currentPosition.x += GetHitbox().width * 0.5f;
	currentPosition.y += GetHitbox().height * 0.5f;
	
	Vector2f
		direction{ playerPos - currentPosition };

	float
		rotationAngle{ (180 / static_cast<float>(M_PI)) * std::atan2(direction.y, direction.x) };


	if (rotationAngle > 90 || rotationAngle < -90)
	{
		rotationAngle -= 180.f;
		m_GunSprite->FlipHorizontally();
		GetSprite()->FlipHorizontally();
	}
	else
	{
		m_GunSprite->ResetHorizontalFlip();
		GetSprite()->ResetHorizontalFlip();
	}


	m_GunSprite->RotateBy(rotationAngle);
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
	float angleRad{
		m_GunSprite->GetRotation() * (static_cast<float>(M_PI) / 180.f)
	};

	Vector2f bulletVelocity{
		std::cosf(angleRad) * bulletSpeed * GetFacingDirection(),
		std::sinf(angleRad) * bulletSpeed * GetFacingDirection()
	};

	particleManager->SpawnBullet(
		AttackParticle::OwnerType::Enemy,
		GetPosition(),
		Vector2f{ 0, GetHitbox().height * 0.6f },
		m_BulletHitbox,
		bulletVelocity,
		m_GunSprite->GetRotation(),
		m_GunSprite->IsFlippedHorizontally(),
		false
	);

	ResetAttackCooldown();

	m_AimTimer = m_AimTime;
}



