#include "pch.h"
#include "ParticleManager.h"
#include "Entity.h"

ParticleManager::ParticleManager(int instanceCount)
{
	m_pParticles.reserve(instanceCount);

	for (int index{ 0 }; index < instanceCount; ++index)
	{
		m_pParticles.push_back(new AttackParticle());
	}
}

ParticleManager::~ParticleManager()
{
	for (AttackParticle* pParticle : m_pParticles)
	{
		delete pParticle;
	}
}

void ParticleManager::Draw() const
{
	for (const AttackParticle* pParticle : m_pParticles)
	{
		if (pParticle->IsActive())
		{
			pParticle->Draw();
		}
	}
}

void ParticleManager::Update(float elapsedSec)
{
	for (AttackParticle* pParticle : m_pParticles)
	{
		if (pParticle->IsActive())
		{
			pParticle->Update(elapsedSec);
		}
	}
}

void ParticleManager::SpawnBullet(
	AttackParticle::OwnerType owner, 
	const Vector2f& position, 
	const Vector2f& positionOffset, 
	const std::vector<Vector2f>& localHitbox,
	const Vector2f& velocity,
	float rotationAngle, 
	bool isFlippedHorizontally, 
	bool isFlippedVertically,
	Sprite* pSprite
) const
{
	AttackParticle* pParticle{
		GetFreeParticle()
	};

	if (pParticle != nullptr)
	{
		pParticle->Spawn(
			owner,
			AttackParticle::AttackType::bullet,
			position,
			positionOffset,
			velocity,
			localHitbox,
			m_FlyingParticleLifeTime,
			rotationAngle,
			isFlippedHorizontally,
			isFlippedVertically,
			nullptr,
			pSprite
		);
	}
}

void ParticleManager::SpawnMelee(
	AttackParticle::OwnerType owner,
	const Entity* pOwnerEntity,
	const Vector2f& position, 
	const Vector2f& positionOffset,
	const std::vector<Vector2f>& localHitbox, 
	float lifetime, 
	float rotationAngle, 
	bool isFlippedHorizontally, 
	bool isFlippedVertically
) const
{
	AttackParticle* pParticle{
		GetFreeParticle()
	};

	if (pParticle != nullptr)
	{
		pParticle->Spawn(
			owner,
			AttackParticle::AttackType::melee,
			position,
			positionOffset,
			Vector2f{},
			localHitbox,
			lifetime,
			rotationAngle,
			isFlippedHorizontally,
			isFlippedVertically,
			pOwnerEntity,
			nullptr
		);
	}
}

void ParticleManager::SpawnThrownObject(
	const Vector2f& position, 
	const Vector2f& velocity,
	Sprite* pSprite
) const
{
	AttackParticle* pParticle{
		GetFreeParticle()
	};

	if (pParticle != nullptr)
	{
		//TODO maybe calculate this hitbox differently
		const Rectf& 
			frameDim{ pSprite->GetCurrentFrameDimensions() };

		float
			halfWidth{ frameDim.width * 0.5f },
			halfHeight{ frameDim.height * 0.5f };

		std::vector<Vector2f> localHitbox;
		localHitbox.reserve(4);

		localHitbox.push_back(Vector2f{ -halfWidth, -halfHeight });
		localHitbox.push_back(Vector2f{ halfWidth, -halfHeight });
		localHitbox.push_back(Vector2f{ halfWidth,  halfHeight });
		localHitbox.push_back(Vector2f{ -halfWidth,  halfHeight });

		pParticle->Spawn(
			AttackParticle::OwnerType::Player,
			AttackParticle::AttackType::thrownObject,
			position,
			Vector2f{0.f, 0.f},
			velocity,
			localHitbox,
			m_FlyingParticleLifeTime,
			0.f,
			false,
			false,
			nullptr,
			pSprite
		);
	}
}

const std::vector<AttackParticle*>& ParticleManager::GetParticles() const
{
	return m_pParticles;
}

AttackParticle* ParticleManager::GetFreeParticle() const
{
	for (AttackParticle* pParticle : m_pParticles)
	{
		if (!pParticle->IsActive())
		{
			return pParticle;
		}
	}
	return nullptr;
}