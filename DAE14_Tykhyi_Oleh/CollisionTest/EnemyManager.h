#pragma once
#include "Grunt.h"
#include "Gangster.h"
#include <vector>

class SpriteManager;
class Map;
class SpriteManager;
class SoundManager;

class EnemyManager final
{
public:
	explicit EnemyManager(const Entity* pTarget, const SoundManager* pSoundManager);
	
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;
	EnemyManager(EnemyManager&&) = delete;
	EnemyManager& operator=(EnemyManager&&) = delete;

	~EnemyManager();

	void Draw() const;

	void Update(float elapsedSec, const Map* gameMap, ParticleManager* particleManager);

	void AddEnemy(Enemy::EnemyType type, const Vector2f& position, float speed, float scale, int floor, SpriteManager* pSpriteManager);

	void InitializeEnemyType(
		Enemy::EnemyType type,
		Sprite* pSpritesheet,
		Sprite* pAdditionalSprite, 
		Sprite* pProjectileSprite,
		const std::vector<AnimationFrameInfo>& animationFrameInfo,
		const std::vector<AnimationFrameInfo>& additionSpriteFrameInfo,
		float playerDetectionRange,
		float attackRange
	);

	void SetControlPoints(const std::vector<ControlPoint>& controlPoints);

	const std::vector<Enemy*>& GetEnemies();

	void ResetEnemies() const;

	void ApplySnapshots(const std::vector<EnemySnapshot>& snapshots);
private:
	struct EnemyTypeTemplate {
		Sprite* spriteSheet{};
		Sprite* additionSprite{};//attack, gun etc
		Sprite* projectileSprite{};
		std::vector<AnimationFrameInfo> enemyAnimationFrameInfo{};
		std::vector<AnimationFrameInfo> additionalSpriteFrameInfo{};
		float playerDetectionRange{};
		float attackRange{};
	};

	const Entity*
		m_pTarget{};

	const SoundManager*
		m_pSoundManager{};

	std::vector<Enemy*>				 m_pEnemies{};
	std::vector<EnemyTypeTemplate>   m_EnemyTypeTemplates{};
	std::vector<ControlPoint>		 m_ControlPoints{};
};

