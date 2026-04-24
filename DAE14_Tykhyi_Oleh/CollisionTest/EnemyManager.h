#pragma once
#include "Grunt.h"
#include "Gangster.h"
#include <vector>

class SpriteManager;
class Map;
class CollisionManager;
class SpriteManager;

class EnemyManager final
{
public:
	EnemyManager(const Entity* pTarget);
	
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;

	~EnemyManager();

	void Draw() const;

	void Update(float elapsedSec, const Map* gameMap, ParticleManager* particleManager, const CollisionManager* collisionManager);

	void AddEnemy(Enemy::EnemyType type, const Vector2f& position, float speed, float scale, int floor, SpriteManager* pSpriteManager);

	void InitializeEnemyType(
		Enemy::EnemyType type,
		Sprite* pSpritesheet,
		Sprite* pAdditionalSprite,
		const std::vector<AnimationFrameInfo>& animationFrameInfo,
		float playerDetectionRange,
		float attackRange
	);

	void SetControlPoints(const std::vector<ControlPoint>& controlPoints);

	const std::vector<Enemy*>& GetEnemies();
private:
	struct EnemyTypeTemplate {
		Sprite* spriteSheet{};
		Sprite* additionSprite{};//attack, gun etc
		std::vector<AnimationFrameInfo> enemyAnimationFrameInfo{};
		float playerDetectionRange{};
		float attackRange{};
	};

	const Entity* m_pTarget{};

	std::vector<Enemy*>				 m_pEnemies{};
	std::vector<EnemyTypeTemplate>   m_EnemyTypeTemplates{};
	std::vector<ControlPoint>		 m_ControlPoints{};
};

