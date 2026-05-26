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
	utils::SetColor(Color4f{ 0.3f, 0.3f, 0.3f, 0.3f });
	utils::FillRect(m_Viewport);

	utils::SetColor(Color4f{ 0.0f, 0.0f, 0.0f, 0.15f });

	float scanlineHeight{ 2.0f };
	float gap{ 2.0f };

	for (float y = 0; y < m_Viewport.height; y += (scanlineHeight + gap))
	{
		utils::FillRect(0.0f, y, m_Viewport.width, scanlineHeight);
	}

	m_pReplayIcon->Draw(Vector2f{ m_Viewport.width * 0.1f, m_Viewport.height * 0.9f }, false, false);
}
