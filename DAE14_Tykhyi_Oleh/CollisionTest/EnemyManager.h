#pragma once
#include "Grunt.h"
#include "Gangster.h"
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

	void Update(float elapsedSec, const Vector2f& playerPos, int playerFloor, const Map* gameMap, ParticleManager* particleManager, const CollisionManager* collisionManager);

	void AddEnemy(Enemy::EnemyType type, const Vector2f& position, float speed, float scale, int floor);

	void InitializeEnemyType(
		Enemy::EnemyType type,
		Sprite* pSpritesheet,
		Sprite* pAdditionalSprite,
		const std::vector<AnimationFrameInfo>& animationFrameInfo,
		float playerDetectionRange,
		float attackRange
	);

	void SetControlPoints(const std::vector<ControlPoint>& controlPoints);
private:
	struct EnemyTypeTemplate {
		Sprite* spriteSheet{};
		Sprite* additionSprite{};//attack, gun etc
		std::vector<AnimationFrameInfo> enemyAnimationFrameInfo{};
		float playerDetectionRange{};
		float attackRange{};
	};

	std::vector<Enemy*>				 m_pEnemies{};
	std::vector<EnemyTypeTemplate>   m_EnemyTypeTemplates{};
	std::vector<ControlPoint>		 m_ControlPoints{};
};

