#include "pch.h"
#include "VFX.h"

#include "ParticleManager.h"

void VFX::SpawnDust(int particleCount, const Vector2f&position, const Vector2f& playerVelocity, ParticleManager* pParticleManager)
{
    const int
        minXSpeed{ 50 },
        maxXSpeed{ 250 },
        minYSpeed{ 20 },
        maxYSpeed{ 160 },
        positionMaxOffsetX{ 10 },
        positionMinOffsetX{ -10 };


    float
        particleDirection{ 1.f };
    if (playerVelocity.x > 0.f)
    {
        particleDirection = -1;
    }


    for (int counter{0}; counter < particleCount; ++counter)
    {
        Vector2f
            spawnPos{ position };
        spawnPos.x += static_cast<float>((rand() % (positionMaxOffsetX - positionMinOffsetX)) + positionMinOffsetX);

        float
            horizontalSpeed{ particleDirection * static_cast<float>((rand() % (maxXSpeed - minXSpeed)) + minXSpeed) };
        float
            verticalSpeed{ static_cast<float>((rand() % (maxYSpeed - minYSpeed)) + minYSpeed) };

        Vector2f velocity{
            horizontalSpeed,
            verticalSpeed
        };


       pParticleManager->SpawnCosmeticParticle(ParticleManager::CosmeticParticleType::dust, spawnPos, velocity, 0.35f);
    }
}

void VFX::SpawnBloodFountain(const Vector2f& position, ParticleManager* pParticleManager)
{
    const int
        maxCount{ 3 },
        minCount{ 1 },
        minXSpeed{ -40 },
        maxXSpeed{ 40 },
        minYSpeed{ 150 },
        maxYSpeed{ 300 };

    int
        particleCount{ 1/*(rand() % (maxCount - minCount)) + minCount*/ };

    for (int counter{ 0 }; counter < particleCount; ++counter)
    {
        float
            horizontalSpeed{  static_cast<float>((rand() % (maxXSpeed - minXSpeed)) + minXSpeed) };
        float
            verticalSpeed{ static_cast<float>((rand() % (maxYSpeed - minYSpeed)) + minYSpeed) };

        Vector2f
            velocity{ horizontalSpeed, verticalSpeed };

        float lifetime{ 0.5f + static_cast<float>(rand() % 50) / 100.f };

        pParticleManager->SpawnCosmeticParticle(
            ParticleManager::CosmeticParticleType::blood,
            position,
            velocity,
            lifetime
        );
    }
}
