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


       pParticleManager->SpawnCosmeticParticle(
           CosmeticParticle::CosmeticParticleType::dust, 
           false,
           0.f,
           spawnPos, 
           velocity, 
           0.35f
       );
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
        particleCount{ (rand() % (maxCount - minCount)) + minCount };

    for (int counter{ 0 }; counter < particleCount; ++counter)
    {
        float
            horizontalSpeed{  static_cast<float>((rand() % (maxXSpeed - minXSpeed)) + minXSpeed) };
        float
            verticalSpeed{ static_cast<float>((rand() % (maxYSpeed - minYSpeed)) + minYSpeed) };

        Vector2f
            velocity{ horizontalSpeed, verticalSpeed };

        float lifetime{ 0.1f + static_cast<float>(rand() % 50) / 100.f };

        pParticleManager->SpawnCosmeticParticle(
            CosmeticParticle::CosmeticParticleType::blood,
            true,
            0.f,
            position,
            velocity,
            lifetime
        );
    }
}

void VFX::SpawnBloodSlash(const Vector2f& position, const Vector2f& slashVelocity, ParticleManager* pParticleManager)
{
    const float
        spreadAngle{ 30.f };

    const int
        maxParticleCount{ 10 },
        minParticleCount{ 5 },
        maxSpeed{ 800 },
        minSpeed{ 300 };
    
    int 
        count{ (rand() % (maxParticleCount - minParticleCount)) + minParticleCount };

    float baseAngle{
        atan2f(slashVelocity.y, slashVelocity.x)
    };

    for (int counter{ 0 }; counter < count; ++counter)
    {
        const float
            spread{ spreadAngle * static_cast<float>(M_PI) / 180.f },
            randomOffset{ (static_cast<float>(rand() % 100) / 100.f - 0.5f) *  spread },
            finalAngle{ baseAngle + randomOffset };

        int 
            speed{ (rand() % (maxSpeed - minSpeed)) + minSpeed };

        Vector2f velocity{
             cosf(finalAngle) * speed,
             sinf(finalAngle) * speed
        };

        pParticleManager->SpawnCosmeticParticle(
            CosmeticParticle::CosmeticParticleType::bloodSlash,
            false,
            baseAngle * (180.f / static_cast<float>(M_PI)),
            position,
            velocity,
            0.3f
        );
    }
}

void VFX::SpawnCracks(const Vector2f& position, const Vector2f& surfaceNormal, ParticleManager* pParticleManager)
{
    const int
        particleCount{ 10 };

    const int
        minSpeed{ 300 },
        maxSpeed{ 600 };

    const float
        spreadAngle{ 60.f };

    float
        baseAngle{ atan2f(surfaceNormal.y, surfaceNormal.x) };

    for (int counter{ 0 }; counter < particleCount; ++counter)
    {
        float
            randomOffset{ (static_cast<float>(rand() % 100) / 100.f - 0.5f) * spreadAngle },
            finalAngle{ baseAngle + randomOffset },
            speed{ static_cast<float>(rand() % (maxSpeed - minSpeed) + minSpeed) };


        Vector2f velocity{
            cosf(finalAngle) * speed,
            sinf(finalAngle) * speed * (0.5f * surfaceNormal.y)
        };

        float
            lifeTime{ (static_cast<float>(rand() % 40) / 100.f) + 0.2f };

        pParticleManager->SpawnCosmeticParticle(
            CosmeticParticle::CosmeticParticleType::cracks,
            true,
            0.f,
            position,
            velocity,
            lifeTime
        );
    }

}
