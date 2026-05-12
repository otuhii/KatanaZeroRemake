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

void LevelManager::Update(float elapsedSec, const Uint8* pStates)
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

void LevelManager::ResetLevel(Map* pMap, ParticleManager* pParticleManager)
{
	pMap->Reset();
	m_pEnemyManager->ResetEnemies();
	m_pPlayer->Reset();
	pParticleManager->Reset();

	m_CurrentLevelTime = m_MaxLevelTime;
	m_SlowMotionCurrentDuration = m_SlowMotionMaxDuration;
}
