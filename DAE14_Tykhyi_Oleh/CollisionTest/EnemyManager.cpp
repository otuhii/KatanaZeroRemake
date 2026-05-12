#include "pch.h"
#include "EnemyManager.h"
#include "SpriteManager.h"

#include "Map.h"
#include "CollisionManager.h"

#include "utils.h"

EnemyManager::EnemyManager(const Entity* pTarget, const SoundManager* pSoundManager)
	: m_pTarget{pTarget}, m_pSoundManager{pSoundManager}
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
		//utils::FillEllipse(point.position, controlPointRadius, controlPointRadius);
	}
}

void EnemyManager::Update(float elapsedSec, const Map* gameMap, ParticleManager* particleManager, const CollisionManager* collisionManager)
{
	for (Enemy* pEnemy : m_pEnemies)
	{
		pEnemy->UpdateControlPoints(&m_ControlPoints);
		pEnemy->Update(elapsedSec, particleManager, gameMap, Rectf{});

		collisionManager->HandleMovement(pEnemy, gameMap, elapsedSec, false);
	}
}

void EnemyManager::AddEnemy(Enemy::EnemyType type, const Vector2f& position, float speed, float scale, int floor, SpriteManager* pSpriteManager)
{
	if (type == Enemy::EnemyType::grunt)
	{
		m_pEnemies.push_back(new Grunt{
			pSpriteManager->CreateCopySprite(m_EnemyTypeTemplates[static_cast<int>(type)].spriteSheet),
			pSpriteManager->CreateCopySprite(m_EnemyTypeTemplates[static_cast<int>(type)].additionSprite),
			m_pTarget,
			m_pSoundManager,
			&m_EnemyTypeTemplates[static_cast<int>(type)].enemyAnimationFrameInfo,
			&m_EnemyTypeTemplates[static_cast<int>(type)].additionalSpriteFrameInfo,
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
			pSpriteManager->CreateCopySprite(m_EnemyTypeTemplates[static_cast<int>(type)].spriteSheet),
			pSpriteManager->CreateCopySprite(m_EnemyTypeTemplates[static_cast<int>(type)].additionSprite),
			pSpriteManager->CreateCopySprite(m_EnemyTypeTemplates[static_cast<int>(type)].projectileSprite),
			m_pTarget,
			m_pSoundManager,
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

void EnemyManager::InitializeEnemyType(Enemy::EnemyType type, Sprite* pSpritesheet, Sprite* pAdditionalSprite, Sprite* pProjectileSprite, const std::vector<AnimationFrameInfo>& animationFrameInfo, const std::vector<AnimationFrameInfo>& additionalSpriteFrameInfo, float playerDetectionRange, float attackRange)
{
	m_EnemyTypeTemplates[static_cast<int>(type)] = EnemyTypeTemplate{ pSpritesheet, pAdditionalSprite, pProjectileSprite, animationFrameInfo, additionalSpriteFrameInfo, playerDetectionRange, attackRange };
}

void EnemyManager::SetControlPoints(const std::vector<ControlPoint>& controlPoints)
{
	m_ControlPoints = controlPoints;
}

const std::vector<Enemy*>& EnemyManager::GetEnemies()
{
	return m_pEnemies;
}

void EnemyManager::ResetEnemies() const
{
	for (Enemy* pEnemy : m_pEnemies)
	{
		pEnemy->Reset();
	}
}
