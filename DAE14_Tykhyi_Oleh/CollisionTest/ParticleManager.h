#pragma once
#include "AttackParticle.h"

class ParticleManager final
{
public:
	ParticleManager(int instanceCount);

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
		bool isFlippedVertically
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

	const std::vector<AttackParticle*>& GetParticles() const;

	AttackParticle* GetFreeParticle() const;
private:
	std::vector<AttackParticle*> m_pParticles{};
};

