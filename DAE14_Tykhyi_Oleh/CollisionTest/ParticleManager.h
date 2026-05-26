#pragma once
#include "AttackParticle.h"
#include "CosmeticParticle.h"

class LevelManager;
class SpriteManager;
class Sprite;

class ParticleManager final
{
public:
	ParticleManager(int attackInstanceCount, int cosmeticInstanceCount, SpriteManager* pSpriteManager);

	~ParticleManager();

	ParticleManager(const ParticleManager&) = delete;
	ParticleManager& operator=(const ParticleManager&) = delete;

	void Draw() const;
	void Update(float elapsedSec);

	void SpawnBullet(
		AttackParticle::OwnerType owner,
		const Vector2f& position, 
		const Vector2f& positionOffset, 
		const std::vector<Vector2f>& localHitbox,
		const Vector2f& velocity,
		float rotationAngle,
		bool isFlippedHorizontally, 
		bool isFlippedVertically,
		Sprite* pSprite
	) const;

	void SpawnMelee(
		AttackParticle::OwnerType owner,
		const Entity* pOwnerEntity,
		const Vector2f& position,
		const Vector2f& positionOffset,
		const std::vector<Vector2f>& localHitbox,
		float lifetime,
		float rotationAngle,
		bool isFlippedHorizontally,
		bool isFlippedVertically
	) const;

	void SpawnThrownObject(
		const Vector2f& position,
		const Vector2f& velocity,
		Sprite* pSprite
	) const;

	void SpawnCosmeticParticle(
		CosmeticParticle::CosmeticParticleType type,
		bool applyGravity,
		float rotation,
		const Vector2f& position,
		const Vector2f& velocity,
		float lifeTime
	) const;

	const std::vector<AttackParticle*>& GetAttackParticles() const;

	AttackParticle* GetFreeAttackParticle() const;
	CosmeticParticle* GetFreeCosmeticParticle() const;

	void LinkLevelManager(LevelManager* pLevelManager);

	void Reset();
private:
	std::vector<AttackParticle*> m_pAttackParticles{};
	std::vector<CosmeticParticle*> m_pCosmeticParticles{};

	std::vector<Sprite*> m_pSpriteTemplates{};

	LevelManager*
		m_pLevelManager{};

	const float
		m_FlyingParticleLifeTime{ 5.f };
};

