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

	m_StandartBulletShape.push_back(Vector2f{ -5.f, -2.f });
	m_StandartBulletShape.push_back(Vector2f{ 5.f, -2.f });
	m_StandartBulletShape.push_back(Vector2f{ 5.f, 2.f });
	m_StandartBulletShape.push_back(Vector2f{ -5.f, 2.f });
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

void ParticleManager::SpawnBullet(AttackParticle::OwnerType owner, const Vector2f& position, const Vector2f& positionOffset, const Vector2f& velocity, float rotationAngle, bool isFlippedHorizontally, bool isFlippedVertically)
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
			m_StandartBulletShape,
			5.f,
			rotationAngle,
			isFlippedHorizontally,
			isFlippedVertically,
			nullptr
		);
	}
}

void ParticleManager::SpawnMelee(AttackParticle::OwnerType owner, Entity* pOwnerEntity, const Vector2f& position, const Vector2f& positionOffset, const std::vector<Vector2f>& localHitbox, float lifetime, float rotationAngle, bool isFlippedHorizontally, bool isFlippedVertically)
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
			pOwnerEntity
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