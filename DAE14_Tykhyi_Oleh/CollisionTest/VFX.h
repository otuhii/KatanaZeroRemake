#pragma once

class ParticleManager;

namespace VFX
{
	void SpawnDust(int particleCount, const Vector2f& position, const Vector2f& playerVelocity, ParticleManager* pParticleManager);
	void SpawnBloodFountain(const Vector2f& position, ParticleManager* pParticleManager);
}