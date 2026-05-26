#pragma once
#include "Snapshots.h"

class Player;
class EnemyManager;
class ParticleManager;
class Map;

class LevelManager final
{
public:
	enum class LevelState {
		Gameplay,
		Replay
	};

	LevelManager(Player* pPlayer, EnemyManager* pEnemyManager);

	~LevelManager();

	void Update(float elapsedSec, const Uint8* pStates);

	LevelState GetCurrentState() const;

	float GetTimeMultiplier() const;
	float GetPassTimeRatio() const;
	float GetSlowMotionDurationRatio() const;
	bool IsPlayerAlive() const;
	
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e, Map* pMap, ParticleManager* pParticleManager);

	void TriggerReplay();

	void RecordParticleEvent(ReplayParticleEvent* pEvent);

	void LinkParticleManager(ParticleManager* pParticleManager);
	
	void Forward();
	void Backward();
	//CanFinishLevel
private:
	Player*
		m_pPlayer{};

	EnemyManager*
		m_pEnemyManager{};

	Vector2f
		m_PlayerStartPosition;

	std::vector<ReplayFrame> m_ReplayBuffer{};

	LevelState
		m_State{LevelState::Gameplay};

	ParticleManager*
		m_pParticleManager{};

	int
		m_ReplayFrameIndex{};

	const float
		m_SlowMotionMaxDuration{ 4.f },
		m_SlowMotionRechargeValue{ 0.5f },
		m_MaxLevelTime{ 30.f };

	float
		m_SlowMotionCurrentDuration{ m_SlowMotionMaxDuration },
		m_CurrentLevelTime{ m_MaxLevelTime };

	bool
		m_IsSlowMoActive{ false };

	float
		m_SlowMoTimeMultiplier{ 0.2f };

	void ResetLevel(Map* pMap, ParticleManager* pParticleManager);
	
	void RecordCurrentFrame();
	void PlaybackFrame();

	void ProcessParticleReplayEvents(const ReplayFrame& currentFrame) const;
	
	void ClearReplayBuffer();
};

