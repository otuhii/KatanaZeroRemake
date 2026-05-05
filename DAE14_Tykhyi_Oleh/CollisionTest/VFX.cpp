#include "pch.h"
#include "VFX.h"

#include "ParticleManager.h"

void VFX::SpawnDust(int particleCount, const Vector2f& position, ParticleManager* pParticleManager)
{
    for (int counter{0}; counter < particleCount; ++counter)
    {
        float
            angle{ static_cast<float>(rand() % 360) * (float(M_PI) / 180.f) };

        float
            speed{ static_cast<float>(100 + (rand() % 150)) };

        Vector2f velocity{
            cosf(angle) * speed,
            sinf(angle) * speed
        };

        pParticleManager->SpawnCosmeticParticle(ParticleManager::CosmeticParticleType::dust, position, velocity, 0.3f);
    }
}

void VFX::SpawnBlood(const Vector2f& pos, ParticleManager* pParticleManager)
{
}
