#include "pch.h"
#include "ParticleManager.h"
#include "Entity.h"

#include "SpriteManager.h"
#include "JsonImporter.h"

ParticleManager::ParticleManager(int attackInstanceCount, int cosmeticInstanceCount, SpriteManager* pSpriteManager)
{
	m_pSpriteTemplates.resize(static_cast<size_t>(CosmeticParticle::CosmeticParticleType::count));
	m_pSpriteTemplates[static_cast<int>(CosmeticParticle::CosmeticParticleType::dust)] = pSpriteManager->CreateSprite("img/vfx/dustParticleSprites.png");
	m_pSpriteTemplates[static_cast<int>(CosmeticParticle::CosmeticParticleType::dust)]->SetAnimationFrameInfo(JsonImporter::ImportAnimationFrameObjects("json/dustParticleAnimationInfo.json")[0]);

	m_pSpriteTemplates[static_cast<int>(CosmeticParticle::CosmeticParticleType::blood)] = pSpriteManager->CreateSprite("img/vfx/bloodParticleSprites.png");
	m_pSpriteTemplates[static_cast<int>(CosmeticParticle::CosmeticParticleType::blood)]->SetAnimationFrameInfo(JsonImporter::ImportAnimationFrameObjects("json/bloodParticleAnimationInfo.json")[0]);
	m_pSpriteTemplates[static_cast<int>(CosmeticParticle::CosmeticParticleType::blood)]->SetStatic(true);

	m_pSpriteTemplates[static_cast<int>(CosmeticParticle::CosmeticParticleType::bloodSlash)] = pSpriteManager->CreateSprite("img/vfx/bloodSlashParticleSprites.png");
	m_pSpriteTemplates[static_cast<int>(CosmeticParticle::CosmeticParticleType::bloodSlash)]->SetAnimationFrameInfo(JsonImporter::ImportAnimationFrameObjects("json/bloodSlashParticleAnimationInfo.json")[0]);
	m_pSpriteTemplates[static_cast<int>(CosmeticParticle::CosmeticParticleType::bloodSlash)]->SetStatic(true);

	m_pSpriteTemplates[static_cast<int>(CosmeticParticle::CosmeticParticleType::cracks)] = pSpriteManager->CreateSprite("img/vfx/cracksParticleSprite.png");
	m_pSpriteTemplates[static_cast<int>(CosmeticParticle::CosmeticParticleType::cracks)]->SetAnimationFrameInfo(JsonImporter::ImportAnimationFrameObjects("json/cracksParticleAnimationInfo.json")[0]);
	m_pSpriteTemplates[static_cast<int>(CosmeticParticle::CosmeticParticleType::cracks)]->SetStatic(true);


	m_pAttackParticles.reserve(attackInstanceCount);

	for (int index{ 0 }; index < attackInstanceCount; ++index)
	{
		m_pAttackParticles.push_back(new AttackParticle{});
	}

	m_pCosmeticParticles.reserve(cosmeticInstanceCount);
	
	for (int index{ 0 }; index < cosmeticInstanceCount; ++index)
	{
		m_pCosmeticParticles.push_back(new CosmeticParticle{ pSpriteManager });
	}
}

ParticleManager::~ParticleManager()
{
	for (AttackParticle* pParticle : m_pAttackParticles)
	{
		delete pParticle;
	}

	for (CosmeticParticle* pParticle : m_pCosmeticParticles)
	{
		delete pParticle;
	}
}

void ParticleManager::Draw() const
{
	for (const AttackParticle* pParticle : m_pAttackParticles)
	{
		if (pParticle->IsActive())
		{
			pParticle->Draw();
		}
	}

	for (const CosmeticParticle* pParticle : m_pCosmeticParticles)
	{
		if (pParticle->IsActive())
		{
			pParticle->Draw();
		}
	}
}

void ParticleManager::Update(float elapsedSec)
{
	for (AttackParticle* pParticle : m_pAttackParticles)
	{
		if (pParticle->IsActive())
		{
			pParticle->Update(elapsedSec);
		}
	}


	for (CosmeticParticle* pParticle : m_pCosmeticParticles)
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
		GetFreeAttackParticle()
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
		GetFreeAttackParticle()
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
		GetFreeAttackParticle()
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

void ParticleManager::SpawnCosmeticParticle(CosmeticParticle::CosmeticParticleType type, bool applyGravity, float rotation, const Vector2f& position, const Vector2f& velocity, float lifeTime) const
{
	CosmeticParticle* pParticle{
		GetFreeCosmeticParticle()
	};

	if (pParticle != nullptr)
	{
		if (type == CosmeticParticle::CosmeticParticleType::dust)
		{
			pParticle->Spawn(type, position, velocity, lifeTime, applyGravity, rotation, m_pSpriteTemplates[static_cast<int>(type)], 2.f);
		}
		else if (type == CosmeticParticle::CosmeticParticleType::blood)
		{
			pParticle->Spawn(type, position, velocity, lifeTime, applyGravity, rotation, m_pSpriteTemplates[static_cast<int>(type)], 1.f);

			pParticle->SetRandomFrame();
		}
		else if (type == CosmeticParticle::CosmeticParticleType::bloodSlash)
		{
			pParticle->Spawn(type, position, velocity, lifeTime, applyGravity, rotation, m_pSpriteTemplates[static_cast<int>(type)], 1.5f);

			pParticle->SetRandomFrame();
		}
		else if (type == CosmeticParticle::CosmeticParticleType::cracks)
		{
			pParticle->Spawn(type, position, velocity, lifeTime, applyGravity, rotation, m_pSpriteTemplates[static_cast<int>(type)], 0.5f);;
		}
	}
}

const std::vector<AttackParticle*>& ParticleManager::GetAttackParticles() const
{
	return m_pAttackParticles;
}

AttackParticle* ParticleManager::GetFreeAttackParticle() const
{
	for (AttackParticle* pParticle : m_pAttackParticles)
	{
		if (!pParticle->IsActive())
		{
			return pParticle;
		}
	}
	return nullptr;
}

CosmeticParticle* ParticleManager::GetFreeCosmeticParticle() const
{
	for (CosmeticParticle* pParticle : m_pCosmeticParticles)
	{
		if (!pParticle->IsActive())
		{
			return pParticle;
		}
	}
	return nullptr;
}
