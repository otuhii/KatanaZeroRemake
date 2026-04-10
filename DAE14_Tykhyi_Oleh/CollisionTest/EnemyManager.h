#pragma once
#include "Enemy.h"
#include <vector>

class SpriteManager;

class EnemyManager final
{
public:
	EnemyManager();
	
	~EnemyManager();

	void Draw() const;

	void Update(float elapsedSec);

	void AddEnemy(Enemy::EnemyType type, const Vector2f& position, float speed, float scale);
	void InitializeEnemyType(
		Enemy::EnemyType type,
		Sprite* pSpritesheet,
		const std::vector<AnimationFrameInfo>& animationFrameInfo
	);

private:
	struct EnemyTypeTemplate {
		Sprite* spriteSheet;
		std::vector<AnimationFrameInfo> enemyAnimationFrameInfo;
	};

	std::vector<Enemy*>				 m_pEnemies{};
	std::vector<EnemyTypeTemplate>   m_EnemyTypeTemplates{};
};

