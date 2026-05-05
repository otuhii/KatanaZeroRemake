#pragma once

class ParticleManager;

namespace VFX
{
	void SpawnDust(int particleCount, const Vector2f& position, ParticleManager* pParticleManager);
	void SpawnBlood(const Vector2f& position, ParticleManager* pParticleManager);
}