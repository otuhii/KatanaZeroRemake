#include "pch.h"
#include "ScreenOverlay.h"
#include "LevelManager.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "utils.h"

#include "Hud.h"

ScreenOverlay::ScreenOverlay(SpriteManager* pSpriteManager, const Rectf& viewport, LevelManager* pLevelManager)
	: m_Viewport{viewport}, m_pLevelManager{ pLevelManager }
{
	m_pHud = new Hud{ pSpriteManager, viewport };

	m_pRestartMessage = pSpriteManager->CreateSprite("img/overlay/restart_message.png");
	m_pRestartMessage->SetScale(0.3f);

	m_pReplayIcon = pSpriteManager->CreateSprite("img/overlay/replay_icon.png");
	m_pReplayIcon->SetScale(0.3f);
}

ScreenOverlay::~ScreenOverlay()
{
	delete m_pHud;
}


void ScreenOverlay::Draw() const
{
	if (m_pLevelManager->GetCurrentState() == LevelManager::LevelState::Gameplay)
	{
		if (m_pLevelManager->IsSlowMotionActive())
		{
			DrawSlowmoOverlay();
		}

		m_pHud->Draw(m_pLevelManager);
		if (!m_pLevelManager->IsPlayerAlive())
		{
			DrawRestartMessage();
		}
	}
	else if (m_pLevelManager->GetCurrentState() == LevelManager::LevelState::Replay)
	{
		DrawReplayOverlay();
	}
}

void ScreenOverlay::Update(float elapsedSec, const Uint8* pStates)
{
	m_pHud->Update(elapsedSec, pStates, m_pLevelManager);

	m_TrackingTimer += elapsedSec * 50.0f; 
	if (m_TrackingTimer > m_Viewport.height) m_TrackingTimer = -50.0f;
}

void ScreenOverlay::DrawRestartMessage() const
{
	const int
		maxRectCount{ 5 };

	const float
		startAlpha{0.1f},
		endAlpha{0.2f};

	const float 
		offset{ 15.f };
	
	const float
		baseWidth{ m_Viewport.width * 0.35f },
		baseHeight{ m_Viewport.height * 0.35f };


	for (int counter{ 0 }; counter < maxRectCount; ++counter)
	{
		float percentage{
			static_cast<float>(counter) / (maxRectCount - 1)
		};
		float
			currentAlpha{ startAlpha + (endAlpha - startAlpha) * percentage };

		float
			currentWidth{ baseWidth - (counter * offset * 2) },
			currentHeight{ baseHeight - (counter * offset * 2) };

		Rectf currentRect{
			m_Viewport.width * 0.5f - currentWidth * 0.5f,
			m_Viewport.height * 0.5f - currentHeight * 0.5f,
			currentWidth,
			currentHeight
		};

		utils::SetColor(Color4f{ 0.f, 0.f, 0.f, currentAlpha });
		utils::FillRect(currentRect);
	}

	m_pRestartMessage->Draw(Vector2f{ m_Viewport.width * 0.5f, m_Viewport.height * 0.5f }, true, true);

}

void ScreenOverlay::DrawReplayOverlay() const
{
	float 
		scanlineHeight{ 2.0f },
		gap{ 2.0f };

	utils::SetColor(Color4f{ 0.3f, 0.3f, 0.3f, 0.3f });
	utils::FillRect(m_Viewport);

	utils::SetColor(Color4f{ 0.0f, 0.0f, 0.0f, 0.15f });

	for (float y = 0; y < m_Viewport.height; y += (scanlineHeight + gap))
	{
		utils::FillRect(0.0f, y, m_Viewport.width, scanlineHeight);
	}

	m_pReplayIcon->Draw(Vector2f{ m_Viewport.width * 0.1f, m_Viewport.height * 0.9f }, false, false);
}

void ScreenOverlay::DrawSlowmoOverlay() const
{
	float 
		scanlineHeight{ 1.5f }, 
		gap{ 2.5f };

	utils::SetColor(Color4f{ 0.05f, 0.65f, 0.75f, 0.2f });
	utils::FillRect(m_Viewport);
	utils::SetColor(Color4f{ 0.0f, 0.0f, 0.0f, 0.5f });
	utils::FillRect(0, 0, m_Viewport.width, m_Viewport.height * 0.12f); 
	utils::FillRect(0, m_Viewport.height * 0.88f, m_Viewport.width, m_Viewport.height * 0.12f); 

	utils::SetColor(Color4f{ 0.0f, 0.8f, 0.9f, 0.12f });
	utils::FillRect(0, m_TrackingTimer, m_Viewport.width, 25.0f);

	utils::SetColor(Color4f{ 0.8f, 0.0f, 0.5f, 0.08f });
	utils::FillRect(0, m_TrackingTimer - 12.0f, m_Viewport.width, 15.0f);

	utils::SetColor(Color4f{ 1.0f, 1.0f, 1.0f, 0.1f });
	utils::FillRect(0, m_TrackingTimer + 35.0f, m_Viewport.width, 3.0f);

	utils::SetColor(Color4f{ 0.0f, 0.8f, 0.8f, 0.05f });
	utils::FillRect(0, fmod(m_TrackingTimer * 2.1f, m_Viewport.height), m_Viewport.width, 5.0f);
	utils::FillRect(0, fmod(m_TrackingTimer * 1.5f + 150.f, m_Viewport.height), m_Viewport.width, 2.0f);

	utils::SetColor(Color4f{ 0.0f, 0.0f, 0.0f, 0.25f });
	

	for (float y = 0; y < m_Viewport.height; y += (scanlineHeight + gap))
	{
		utils::FillRect(0.0f, y, m_Viewport.width, scanlineHeight);
	}
}
