#pragma once
#include "Enemy.h"
class Gangster final:
    public Enemy
{
public:
    Gangster(
        Sprite* pSprite,
        Sprite* pGunsprite,
        const std::vector<AnimationFrameInfo>* enemyAnimationFrames,
        const Vector2f& position,
        float speed,
        float scale,
        int floor,
        float playerDetectionRange,
        float attackRange
    );

    void Draw() const override;

    void Update(float elapsedSec, const Vector2f& playerPos, int playerFloor, ParticleManager* particleManager, const Rectf& viewport) override;

    void Attack(const Vector2f& playerPos, ParticleManager* particleManager) override;

private:
    const std::vector<Vector2f> m_BulletHitbox{
        Vector2f{ -5.f, -2.f },
        Vector2f{ 5.f, -2.f },
        Vector2f{ 5.f, 2.f },
        Vector2f{ -5.f, 2.f }
    };

    const float
        m_AimTime{};

    float 
        m_AimTimer{};

    const float
        bulletSpeed{ 600.f };

    Sprite* m_GunSprite{};

    void UpdateAttack(float elapsedSec, const Vector2f& playerPos, ParticleManager* particleManager) override;
    void UpdateGunSprite();

    void Aim(const Vector2f& playerPos);
    void UpdateAimTime(float elapsedSec);
    
    void Shoot(ParticleManager* particleManager);
};

