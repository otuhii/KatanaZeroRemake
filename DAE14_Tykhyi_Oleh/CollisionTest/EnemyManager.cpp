#include "pch.h"
#include "EnemyManager.h"
#include "SpriteManager.h"

#include "Map.h"
#include "CollisionManager.h"

#include "utils.h"

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

	for (const ControlPoint& point : m_ControlPoints)
	{
		const float
			controlPointRadius{ 16.f };

		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
		utils::FillEllipse(point.position, controlPointRadius, controlPointRadius);
	}
}

void EnemyManager::Update(float elapsedSec, const Vector2f& playerPos, int playerFloor, const Map* gameMap, const CollisionManager* collisionManager)
{
	for (Enemy* pEnemy : m_pEnemies)
	{
		pEnemy->UpdateControlPoints(&m_ControlPoints);
		pEnemy->Update(elapsedSec, playerPos, playerFloor, Rectf{});

		collisionManager->HandleMovement(pEnemy, *gameMap, elapsedSec, false);
	}

}

void EnemyManager::AddEnemy(Enemy::EnemyType type, const Vector2f& position, float speed, float scale, int floor)
{
	if (type == Enemy::EnemyType::grunt)
	{
		m_pEnemies.push_back(new Grunt{
			m_EnemyTypeTemplates[static_cast<int>(type)].spriteSheet,
			&m_EnemyTypeTemplates[static_cast<int>(type)].enemyAnimationFrameInfo,
			position,
			speed,
			scale,
			floor,
			m_EnemyTypeTemplates[static_cast<int>(type)].playerDetectionRange,
			m_EnemyTypeTemplates[static_cast<int>(type)].attackRange
			});
	}
	else if (type == Enemy::EnemyType::gangster)
	{
		m_pEnemies.push_back(new Gangster{
			m_EnemyTypeTemplates[static_cast<int>(type)].spriteSheet,
			&m_EnemyTypeTemplates[static_cast<int>(type)].enemyAnimationFrameInfo,
			position,
			speed,
			scale,
			floor,
			m_EnemyTypeTemplates[static_cast<int>(type)].playerDetectionRange,
			m_EnemyTypeTemplates[static_cast<int>(type)].attackRange
			});
	}
}

void EnemyManager::InitializeEnemyType(Enemy::EnemyType type, Sprite* pSpritesheet, const std::vector<AnimationFrameInfo>& animationFrameInfo, float playerDetectionRange, float attackRange)
{
	m_EnemyTypeTemplates[static_cast<int>(type)] = EnemyTypeTemplate{ pSpritesheet, animationFrameInfo, playerDetectionRange, attackRange };
}

void EnemyManager::SetControlPoints(const std::vector<ControlPoint>& controlPoints)
{
	m_ControlPoints = controlPoints;
}
