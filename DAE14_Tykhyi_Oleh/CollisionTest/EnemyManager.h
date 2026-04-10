#pragma once
#include "Enemy.h"
#include <vector>
#include "JsonImporter.h"

class SpriteManager;

class EnemyManager final
{
public:
	EnemyManager(SpriteManager* m_pSpriteManager, const JsonImporter& jsonImporter);
	
	~EnemyManager();

	void Draw() const;

	void Update(float elapsedSec);

	void AddEnemy(Enemy::EnemyType type, const Vector2f& position, float speed, float scale);
private:
	struct EnemyTypeTemplate {
		Sprite* spriteSheet;
		std::vector<AnimationFrameInfo> enemyAnimationFrameInfo;
	};

	std::vector<Enemy*>				 m_pEnemies{};
	std::vector<EnemyTypeTemplate>   m_EnemyTypeTemplates{};

	void LoadSpriteSheets(SpriteManager* m_pSpriteManager);
	void LoadAnimationFrames(const JsonImporter& jsonImporter);
};

