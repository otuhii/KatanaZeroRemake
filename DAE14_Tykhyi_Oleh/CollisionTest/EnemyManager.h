#pragma once
#include "Enemy.h"
#include <vector>

class SpriteManager;
class Map;
class CollisionManager;

class EnemyManager final
{
public:
	EnemyManager();
	
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;

	~EnemyManager();

	void Draw() const;

	void Update(float elapsedSec, const Vector2f& playerPos, int playerFloor, const Map* gameMap, const CollisionManager* collisionManager);

	void AddEnemy(Enemy::EnemyType type, const Vector2f& position, float speed, float scale, int floor);

	void InitializeEnemyType(
		Enemy::EnemyType type,
		Sprite* pSpritesheet,
		const std::vector<AnimationFrameInfo>& animationFrameInfo
	);

	void SetControlPoints(const std::vector<ControlPoint>& controlPoints);
private:
	struct EnemyTypeTemplate {
		Sprite* spriteSheet{};
		std::vector<AnimationFrameInfo> enemyAnimationFrameInfo{};
	};

	std::vector<Enemy*>				 m_pEnemies{};
	std::vector<EnemyTypeTemplate>   m_EnemyTypeTemplates{};
	std::vector<ControlPoint>		 m_ControlPoints{};
};

