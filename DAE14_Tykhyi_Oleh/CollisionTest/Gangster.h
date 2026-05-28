#pragma once
#include "Enemy.h"
class Gangster final:
    public Enemy
{
public:
    explicit Gangster(
        Sprite* pSprite,
        Sprite* pGunsprite,
        Sprite* pBulletProjectile,
        const Entity* pTarget,
        const SoundManager* pSoundManager,
        const std::vector<AnimationFrameInfo>* enemyAnimationFrames,
        const Vector2f& position,
        float speed,
        float scale,
        int floor,
        float playerDetectionRange,
        float attackRange
    );

    void Draw() const override;

    void Update(float elapsedSec, ParticleManager* particleManager, const Map* pMap, const Rectf& viewport) override;

    void Attack(ParticleManager* particleManager) override;

private:
    const std::vector<Vector2f> m_BulletHitbox{
        Vector2f{ -5.f, -2.f },
        Vector2f{ 5.f, -2.f },
        Vector2f{ 5.f, 2.f },
        Vector2f{ -5.f, 2.f }
    };

    const float
        m_AimTime{0.45f};

    float 
        m_AimTimer{m_AimTime};

    const float
        bulletSpeed{ 1200.f };

    Sprite*
        m_pGunSprite{};

    Sprite*
        m_pBulletProjectileSprite{};

    void UpdateCurrentState(float elapsedSec, ParticleManager* particleManager, const Map* pMap) override;

    void UpdateAttack(float elapsedSec, ParticleManager* particleManager) override;
    void UpdateGunSprite();

    void Aim();
    void UpdateAimTime(float elapsedSec);
    
    void Shoot(ParticleManager* particleManager);
};

