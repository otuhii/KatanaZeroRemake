#include "pch.h"
#include "LevelManager.h"

#include "Player.h"
#include "Map.h"
#include "EnemyManager.h"
#include "ParticleManager.h"


LevelManager::LevelManager(Player* pPlayer, EnemyManager* pEnemyManager)
	: m_pPlayer{ pPlayer }, m_pEnemyManager{pEnemyManager}
{
}

LevelManager::~LevelManager()
{
	ClearReplayBuffer();
}

void LevelManager::Update(float elapsedSec, const Uint8* pStates)
{
	if (m_State == LevelState::Gameplay)
	{
		m_CurrentLevelTime -= elapsedSec;
		if (m_CurrentLevelTime <= 0)
		{
			m_CurrentLevelTime = 0.f;
			if (m_pPlayer->IsAlive())
			{
				m_pPlayer->Kill(Vector2f{});
			}
		}

		m_IsSlowMoActive = pStates[SDL_SCANCODE_LSHIFT];

		if (m_IsSlowMoActive && m_SlowMotionCurrentDuration > 0)
		{
			m_SlowMotionCurrentDuration -= elapsedSec;
			if (m_SlowMotionCurrentDuration <= 0)
			{
				m_IsSlowMoActive = false;
				m_SlowMotionCurrentDuration = 0.f;
			}
		}
		else
		{
			m_IsSlowMoActive = false;
			if (m_SlowMotionCurrentDuration < m_SlowMotionMaxDuration)
			{

				m_SlowMotionCurrentDuration += elapsedSec * m_SlowMotionRechargeValue;
			}
		}

		//replay
		RecordCurrentFrame();

		if (!IsPlayerAlive())
		{
			ClearReplayBuffer();
		}
	}
	else if (m_State == LevelState::Replay)
	{
		PlaybackFrame();
	}
	

}

LevelManager::LevelState LevelManager::GetCurrentState() const
{
	return m_State;
}

float LevelManager::GetTimeMultiplier() const
{
	if (m_IsSlowMoActive)
	{
		return m_SlowMoTimeMultiplier;
	}
	else
	{
		return 1.f;
	}
}

float LevelManager::GetPassTimeRatio() const
{
	return m_CurrentLevelTime/m_MaxLevelTime;
}

float LevelManager::GetSlowMotionDurationRatio() const
{
	return (m_SlowMotionCurrentDuration / m_SlowMotionMaxDuration);
}

bool LevelManager::IsPlayerAlive() const
{
	return m_pPlayer->IsAlive();
}

void LevelManager::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e, Map* pMap, ParticleManager* pParticleManager)
{
	if (!m_pPlayer->IsAlive())
	{
		if (e.button == SDL_BUTTON_LEFT)
		{
			ResetLevel(pMap, pParticleManager);
		}
	}
}

void LevelManager::TriggerReplay()
{
	m_State = LevelState::Replay;
	m_ReplayFrameIndex = 0;
}

void LevelManager::RecordParticleEvent(ReplayParticleEvent* pEvent)
{
	if (m_ReplayBuffer.empty() || m_State != LevelState::Gameplay)
	{
		return;
	}

	m_ReplayBuffer.back().particleEvents.push_back(pEvent);

}

void LevelManager::LinkParticleManager(ParticleManager* pParticleManager)
{
	m_pParticleManager = pParticleManager;
}

void LevelManager::RecordCurrentFrame()
{
	ReplayFrame frame{};

	frame.player.position = m_pPlayer->GetPosition();
	frame.player.state = m_pPlayer->GetState();
	frame.player.currentFrame = m_pPlayer->GetCurrentFrame();
	frame.player.isFlipped = (m_pPlayer->GetFacingDirection() == -1);
	frame.player.splashRotation = m_pPlayer->GetSplashRotation();
	frame.player.isDrawingSplash = m_pPlayer->IsSplashDrawn();
	frame.player.currentSplashFrame = m_pPlayer->GetSplashAnimationFrame();

	const std::vector<Enemy*>& enemies = m_pEnemyManager->GetEnemies();

	for (Enemy* pEnemy : enemies)
	{
		EnemySnapshot enemySnap{};
		enemySnap.position = pEnemy->GetPosition();
		enemySnap.state = pEnemy->GetState();

		enemySnap.currentFrame = pEnemy->GetCurrentFrame();
		enemySnap.isFlipped = (pEnemy->GetFacingDirection() == -1);

		frame.enemies.push_back(enemySnap);
	}

	m_ReplayBuffer.push_back(frame);
}

void LevelManager::PlaybackFrame()
{
	if (m_ReplayFrameIndex >= m_ReplayBuffer.size())
	{
		m_ReplayFrameIndex = 0;
		return;
	}
	
	const ReplayFrame& currentFrame = m_ReplayBuffer[m_ReplayFrameIndex];

	m_pPlayer->ApplySnapshot(&currentFrame.player);
	m_pEnemyManager->ApplySnapshots(currentFrame.enemies);
	ProcessParticleReplayEvents(currentFrame);

	m_ReplayFrameIndex++;
}

void LevelManager::ProcessParticleReplayEvents(const ReplayFrame& currentFrame) const
{
	if (m_pParticleManager)
	{
		for (ReplayParticleEvent* pEvent : currentFrame.particleEvents)
		{
			AttackParticle* pAttackPart{ nullptr };

			switch (pEvent->type)
			{
			case ReplayParticleType::bullet:
			{
				pAttackPart = m_pParticleManager->GetFreeAttackParticle();
				if (pAttackPart)
				{
					pAttackPart->Spawn(
						AttackParticle::OwnerType::none,
						AttackParticle::AttackType::bullet,
						pEvent->position,
						pEvent->positionOffset,
						pEvent->velocity,
						{},
						pEvent->lifetime,
						pEvent->rotationAngle,
						pEvent->isFlippedHorizontally,
						pEvent->isFlippedVertically,
						nullptr,
						pEvent->pSprite);
				}
				break;
			}
			case ReplayParticleType::thrownObject:
				pAttackPart = m_pParticleManager->GetFreeAttackParticle();
				if (pAttackPart)
				{
					pAttackPart->Spawn(
						AttackParticle::OwnerType::none,
						AttackParticle::AttackType::thrownObject,
						pEvent->position,
						pEvent->positionOffset,
						pEvent->velocity,
						{},
						pEvent->lifetime,
						pEvent->rotationAngle,
						pEvent->isFlippedHorizontally,
						pEvent->isFlippedVertically,
						nullptr,
						pEvent->pSprite);
				}
				break;
			}
		}
	}
}

void LevelManager::ClearReplayBuffer()
{
	for (ReplayFrame& frame : m_ReplayBuffer)
	{
		for (ReplayParticleEvent* pEvent : frame.particleEvents)
		{
			delete pEvent;
		}
		frame.particleEvents.clear();
	}
	m_ReplayBuffer.clear();
}

void LevelManager::ResetLevel(Map* pMap, ParticleManager* pParticleManager)
{
	pMap->Reset();
	m_pEnemyManager->ResetEnemies();
	m_pPlayer->Reset();
	pParticleManager->Reset();

	m_CurrentLevelTime = m_MaxLevelTime;
	m_SlowMotionCurrentDuration = m_SlowMotionMaxDuration;

	ClearReplayBuffer();
}
