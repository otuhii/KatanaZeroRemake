#include "pch.h"
#include "ScreenOverlay.h"
#include "LevelManager.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "utils.h"

ScreenOverlay::ScreenOverlay(SpriteManager* pSpriteManager)
{
	m_pRestartMessage = pSpriteManager->CreateSprite("img/overlay/restart_message.png");
	m_pRestartMessage->SetScale(0.3f);
}


void ScreenOverlay::Draw(const LevelManager* pLevelManager, const Rectf& viewport) const
{
	if (!pLevelManager->IsPlayerAlive())
	{
		DrawRestartMessage(viewport);
	}

}

void ScreenOverlay::DrawRestartMessage(const Rectf& viewport) const
{
	const int
		maxRectCount{ 5 };

	const float
		startAlpha{0.1f},
		endAlpha{0.2f};

	const float 
		offset{ 15.f };
	
	const float
		baseWidth{ viewport.width * 0.35f },
		baseHeight{ viewport.height * 0.35f };


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
			viewport.width * 0.5f - currentWidth * 0.5f,
			viewport.height * 0.5f - currentHeight * 0.5f,
			currentWidth,
			currentHeight
		};

		utils::SetColor(Color4f{ 0.f, 0.f, 0.f, currentAlpha });
		utils::FillRect(currentRect);
	}


	m_pRestartMessage->Draw(Vector2f{ viewport.width * 0.5f, viewport.height * 0.5f }, true, true);

}
