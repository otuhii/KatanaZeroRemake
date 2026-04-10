#include "pch.h"
#include "EnemyManager.h"
#include "SpriteManager.h"

EnemyManager::EnemyManager(SpriteManager* m_pSpriteManager, const JsonImporter& jsonImporter)
{
	size_t typeOptionCount{
		static_cast<size_t>(Enemy::EnemyType::count)
	};

	m_EnemyTypeTemplates.resize(typeOptionCount);

	LoadSpriteSheets(m_pSpriteManager);
	LoadAnimationFrames(jsonImporter);
}

EnemyManager::~EnemyManager()
{
	for (Enemy* pEnemy : m_pEnemies)
	{
		delete pEnemy;
	}
}

void EnemyManager::Draw() const
{
	for (const Enemy* pEnemy : m_pEnemies)
	{
		pEnemy->Draw();
	}
}

void EnemyManager::Update(float elapsedSec)
{
	for (Enemy* pEnemy : m_pEnemies)
	{
		pEnemy->Update(elapsedSec, Rectf{});
	}
}

void EnemyManager::AddEnemy(Enemy::EnemyType type, const Vector2f& position, float speed, float scale)
{
	m_pEnemies.push_back(new Enemy{
		type, 
		m_EnemyTypeTemplates[static_cast<int>(type)].spriteSheet,
		m_EnemyTypeTemplates[static_cast<int>(type)].enemyAnimationFrameInfo,
		position, 
		speed, 
		scale 
	});
}

void EnemyManager::LoadSpriteSheets(SpriteManager* m_pSpriteManager)
{
	m_EnemyTypeTemplates[static_cast<int>(Enemy::EnemyType::grunt)].spriteSheet = m_pSpriteManager->CreateSprite("img/grunt_spritesheet.png");
	m_EnemyTypeTemplates[static_cast<int>(Enemy::EnemyType::gangster)].spriteSheet = m_pSpriteManager->CreateSprite("img/gangster_spritesheet.png");
}

void EnemyManager::LoadAnimationFrames(const JsonImporter& jsonImporter)
{
	m_EnemyTypeTemplates[static_cast<int>(Enemy::EnemyType::grunt)].enemyAnimationFrameInfo = jsonImporter.ImportAnimationFrameObjects("json/enemy_Grunt_FrameInfo.json");
	m_EnemyTypeTemplates[static_cast<int>(Enemy::EnemyType::gangster)].enemyAnimationFrameInfo = jsonImporter.ImportAnimationFrameObjects("json/enemy_Gangster_FrameInfo.json");
}
