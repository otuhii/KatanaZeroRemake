#pragma once

class ParticleManager;

namespace VFX
{
	void SpawnDust(int particleCount, const Vector2f& position, const Vector2f& playerVelocity, ParticleManager* pParticleManager);
	void SpawnBloodFountain(const Vector2f& position, ParticleManager* pParticleManager);
	void SpawnBloodSlash(const Vector2f& position, const Vector2f& slashVelocity, ParticleManager* pParticleManager);
	void SpawnCracks(const Vector2f& position, const Vector2f& surfaceNormal, ParticleManager* pParticleManager);
}