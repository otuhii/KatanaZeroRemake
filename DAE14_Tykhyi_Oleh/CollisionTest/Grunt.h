#pragma once
#include "Enemy.h"
class Grunt final:
    public Enemy
{
public:
    Grunt(
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

