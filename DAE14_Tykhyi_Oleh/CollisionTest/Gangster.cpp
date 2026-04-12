#include "pch.h"
#include "Gangster.h"

Gangster::Gangster(
	Sprite* pSprite,
	const std::vector<AnimationFrameInfo>* enemyAnimationFrames, 
	const Vector2f& position,
	float speed, 
	float scale, 
	int floor, 
	float playerDetectionRange,
	float attackRange)
	:Enemy{
		EnemyType::gangster,
		pSprite,
		enemyAnimationFrames,
		position,
		speed,
		scale,
		floor,
		playerDetectionRange,
		attackRange
	}
{
}

void Gangster::Attack()
{
}
