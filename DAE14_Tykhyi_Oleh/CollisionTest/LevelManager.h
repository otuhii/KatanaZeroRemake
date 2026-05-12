#pragma once

class Player;
class EnemyManager;
class ParticleManager;
class Map;

class LevelManager
{
public:
	LevelManager(Player* pPlayer, EnemyManager* pEnemyManager);

	void Update(float elapsedSec, const Uint8* pStates);

	float GetTimeMultiplier() const;
	float GetPassTimeRatio() const;
	float GetSlowMotionDurationRatio() const;
	bool IsPlayerAlive() const;
	
	

	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e, Map* pMap, ParticleManager* pParticleManager);

	//CanFinishLevel
private:
	Player*
		m_pPlayer{};

	EnemyManager*
		m_pEnemyManager{};

	Vector2f
		m_PlayerStartPosition;

	const float
		m_SlowMotionMaxDuration{ 4.f },
		m_SlowMotionRechargeValue{ 0.5f },
		m_MaxLevelTime{ 15.f };

	float
		m_SlowMotionCurrentDuration{ m_SlowMotionMaxDuration },
		m_CurrentLevelTime{ m_MaxLevelTime };

	bool
		m_IsSlowMoActive{ false };

	float
		m_SlowMoTimeMultiplier{ 0.2f };

	void ResetLevel(Map* pMap, ParticleManager* pParticleManager);

};

