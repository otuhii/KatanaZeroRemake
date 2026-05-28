#pragma once
#include "Snapshots.h"

class Player;
class EnemyManager;
class ParticleManager;
class SoundManager;
class Map;

class LevelManager final
{
public:
	enum class LevelState {
		Gameplay,
		Replay
	};

	explicit LevelManager(Player* pPlayer, EnemyManager* pEnemyManager);

	~LevelManager();

	LevelManager(const LevelManager&) = delete;
	LevelManager& operator=(const LevelManager&) = delete;
	LevelManager(LevelManager&&) = delete;
	LevelManager& operator=(LevelManager&&) = delete;

	void Update(float elapsedSec, const Uint8* pStates);

	LevelState GetCurrentState() const;

	float GetTimeMultiplier() const;
	float GetPassTimeRatio() const;
	float GetSlowMotionDurationRatio() const;
	bool IsPlayerAlive() const;
	
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);

	void TriggerReplay();

	void RecordParticleEvent(ReplayParticleEvent* pEvent);
	void RecordSound(SoundManager::SoundEffectType type);

	void LinkParticleManager(ParticleManager* pParticleManager);
	void LinkSoundManager(SoundManager* pSoundManager);
	void LinkMap(Map* pMap);
	
	Player* GetPlayer() const;

	void Forward();
	void Backward();

private:
	Player*
		m_pPlayer{};

	EnemyManager*
		m_pEnemyManager{};

	SoundManager*
		m_pSoundManager{};

	Map*
		m_pMap{};

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

	void ResetLevel();
	
	void RecordCurrentFrame();
	void PlaybackFrame();
	void RecordInteractable(ReplayFrame& currentFrame);

	void ProcessParticleReplayEvents(const ReplayFrame& currentFrame) const;
	void ProcessSoundReplay(const ReplayFrame& currentFrame) const;
	void ProcessInteractableObjectReplay(const ReplayFrame& currentFrame) const;
	
	void ClearReplayBuffer();

	bool CanFinishLevel() const;

};

