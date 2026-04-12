#include "pch.h"
#include "Grunt.h"

Grunt::Grunt(
	Sprite* pSprite,
	const std::vector<AnimationFrameInfo>* enemyAnimationFrames,
	const Vector2f& position,
	float speed,
	float scale,
	int floor,
	float playerDetectionRange,
	float attackRange)
	:Enemy{
		EnemyType::grunt,
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

void Grunt::Attack()
{

}
