#include "pch.h"
#include "EnemyManager.h"
#include "SpriteManager.h"

EnemyManager::EnemyManager()
{
	size_t typeOptionCount{
		static_cast<size_t>(Enemy::EnemyType::count)
	};
	m_EnemyTypeTemplates.resize(typeOptionCount);
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

void EnemyManager::InitializeEnemyType(Enemy::EnemyType type, Sprite* pSpritesheet, const std::vector<AnimationFrameInfo>& animationFrameInfo)
{
	m_EnemyTypeTemplates[static_cast<int>(type)] = EnemyTypeTemplate{ pSpritesheet, animationFrameInfo };
}
