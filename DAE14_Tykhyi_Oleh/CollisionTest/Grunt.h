#pragma once
#include "Enemy.h"
class Grunt final:
    public Enemy
{
public:
    Grunt(
        Sprite* pSprite,
        const Entity* pTarget,
        const std::vector<AnimationFrameInfo>* enemyAnimationFrames,
        const Vector2f& position,
        float speed,
        float scale,
        int floor,
        float playerDetectionRange,
        float attackRange
    );

    void Attack(ParticleManager* particleManager) override;

private:
    const std::vector<Vector2f> m_BaseAttackHitbox{
        Vector2f{-30.f, -20.f},
        Vector2f{45.f, -20.f},
        Vector2f{45.f, 20.f},
        Vector2f{-30.f, 20.f}
    };

    void SpawnAttackParticle(ParticleManager* particleManager) const;
};

