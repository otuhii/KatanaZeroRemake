#pragma once
#include "Enemy.h"
class Gangster final:
    public Enemy
{
public:
    Gangster(
        Sprite* pSprite,
        const std::vector<AnimationFrameInfo>* enemyAnimationFrames,
        const Vector2f& position,
        float speed,
        float scale,
        int floor,
        float playerDetectionRange,
        float attackRange
    );

    void Attack() override;
};

