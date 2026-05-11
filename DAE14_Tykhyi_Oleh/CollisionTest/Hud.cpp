#include "pch.h"
#include "Hud.h"

#include "SpriteManager.h"
#include "Player.h"
#include "ThrowableObject.h"


Hud::Hud(Player* pPlayer, SpriteManager* pSpriteManager)
	: m_pPlayer{pPlayer}
{
	const float
		scale{ 2.f };

	m_pHudVisuals.resize(static_cast<int>(HudPartSprites::count));

	m_pHudVisuals[static_cast<int>(HudPartSprites::battery)] = pSpriteManager->CreateSprite("img/hud/spr_hud_battery.png");
	m_pHudVisuals[static_cast<int>(HudPartSprites::betteryFillPart)] = pSpriteManager->CreateSprite("img/hud/spr_battery_part.png");
	m_pHudVisuals[static_cast<int>(HudPartSprites::hudBar)] = pSpriteManager->CreateSprite("img/hud/spr_hud.png");
	m_pHudVisuals[static_cast<int>(HudPartSprites::subWeaponTexture)] = pSpriteManager->CreateSprite("img/hud/spr_hud_subweapon.png");
	m_pHudVisuals[static_cast<int>(HudPartSprites::timerFill)] = pSpriteManager->CreateSprite("img/hud/spr_timer.png");
	m_pHudVisuals[static_cast<int>(HudPartSprites::timerBox)] = pSpriteManager->CreateSprite("img/hud/spr_hud_timer.png");
	m_pHudVisuals[static_cast<int>(HudPartSprites::spacePressed)] = pSpriteManager->CreateSprite("img/hud/spr_spacebar_1.png");
	m_pHudVisuals[static_cast<int>(HudPartSprites::spaceReleased)] = pSpriteManager->CreateSprite("img/hud/spr_spacebar_0.png");
	m_pHudVisuals[static_cast<int>(HudPartSprites::shiftPressed)] = pSpriteManager->CreateSprite("img/hud/spr_keyboard_shift_1.png");
	m_pHudVisuals[static_cast<int>(HudPartSprites::shiftReleased)] = pSpriteManager->CreateSprite("img/hud/spr_keyboard_shift_0.png");
	m_pHudVisuals[static_cast<int>(HudPartSprites::leftClick)] = pSpriteManager->CreateSprite("img/hud/spr_left_click.png");
	m_pHudVisuals[static_cast<int>(HudPartSprites::katanaIcon)] = pSpriteManager->CreateSprite("img/hud/spr_katanaicons_2.png");
	m_pHudVisuals[static_cast<int>(HudPartSprites::pickedItem)] = pSpriteManager->CreateSprite("img/hud/spr_itemicons_0.png");

	for (Sprite* pSprite : m_pHudVisuals)
	{
		pSprite->SetScale(scale);
	}

	m_pHudVisuals[static_cast<int>(HudPartSprites::spacePressed)]->SetScale(1.5f);
	m_pHudVisuals[static_cast<int>(HudPartSprites::spaceReleased)]->SetScale(1.5f);


	m_TimerFullWidth = m_pHudVisuals[static_cast<int>(HudPartSprites::timerFill)]->GetCurrentFrameDimensions().width;
}

void Hud::Draw(const Rectf& screenDimensions) const
{
	float
		scale{ m_pHudVisuals[static_cast<int>(HudPartSprites::hudBar)]->GetScale() }; //uniform scaling

	Rectf
		barScreenDimensions{ m_pHudVisuals[static_cast<int>(HudPartSprites::hudBar)]->GetCurrentFrameDimensions() };

	Vector2f barPosition{
		0.f,
		screenDimensions.height - barScreenDimensions.height * scale
	};
	Vector2f timerPosition{
		barPosition.x + barScreenDimensions.width * 0.5f * scale - m_pHudVisuals[static_cast<int>(HudPartSprites::timerBox)]->GetCurrentFrameDimensions().width * scale * 0.5f,
		barPosition.y
	};
	Vector2f timerFillPosition{
		timerPosition.x + (17.f * scale), 
		barPosition.y + 12.f  
	};

	Vector2f batteryPosition{
		barPosition.x, 
		barPosition.y
	};

	m_pHudVisuals[static_cast<int>(HudPartSprites::hudBar)]->Draw(barPosition, false, false);
	m_pHudVisuals[static_cast<int>(HudPartSprites::timerBox)]->Draw(timerPosition, false, false);
	m_pHudVisuals[static_cast<int>(HudPartSprites::timerFill)]->Draw(timerFillPosition, false, false);
	m_pHudVisuals[static_cast<int>(HudPartSprites::battery)]->Draw(batteryPosition, false, false);

	if (m_IsShiftPressed)
	{
		m_pHudVisuals[static_cast<int>(HudPartSprites::shiftPressed)]->Draw(Vector2f{batteryPosition.x + m_pHudVisuals[static_cast<int>(HudPartSprites::battery)]->GetCurrentFrameDimensions().width * scale, batteryPosition.y + 7.f}, false, false);
	}
	else
	{
		m_pHudVisuals[static_cast<int>(HudPartSprites::shiftReleased)]->Draw(Vector2f{ batteryPosition.x + m_pHudVisuals[static_cast<int>(HudPartSprites::battery)]->GetCurrentFrameDimensions().width * scale, batteryPosition.y + 7.f}, false, false);
	}

	Vector2f subWeaponSignifierPosition{
		barPosition.x + screenDimensions.width - m_pHudVisuals[static_cast<int>(HudPartSprites::subWeaponTexture)]->GetCurrentFrameDimensions().width * scale - 25.f,
		barPosition.y
	};
	m_pHudVisuals[static_cast<int>(HudPartSprites::subWeaponTexture)]->Draw(subWeaponSignifierPosition, false, false);


	m_pHudVisuals[static_cast<int>(HudPartSprites::katanaIcon)]->Draw(Vector2f{ subWeaponSignifierPosition.x , subWeaponSignifierPosition.y }, false, false);


	m_pHudVisuals[static_cast<int>(HudPartSprites::pickedItem)]->Draw(Vector2f{ subWeaponSignifierPosition.x + m_pHudVisuals[static_cast<int>(HudPartSprites::subWeaponTexture)]->GetCurrentFrameDimensions().width * 0.6f * scale, subWeaponSignifierPosition.y }, false, false);

	m_pHudVisuals[static_cast<int>(HudPartSprites::leftClick)]->Draw(Vector2f{ subWeaponSignifierPosition.x + m_pHudVisuals[static_cast<int>(HudPartSprites::subWeaponTexture)]->GetCurrentFrameDimensions().width * 0.35f * scale, subWeaponSignifierPosition.y - 15.f }, false, false);



	if (m_IsSpacePressed)
	{
		m_pHudVisuals[static_cast<int>(HudPartSprites::spacePressed)]->Draw(Vector2f{ subWeaponSignifierPosition.x + m_pHudVisuals[static_cast<int>(HudPartSprites::subWeaponTexture)]->GetCurrentFrameDimensions().width * 0.8f * scale, subWeaponSignifierPosition.y - 15.f }, false, false);
	}
	else
	{
		m_pHudVisuals[static_cast<int>(HudPartSprites::spaceReleased)]->Draw(Vector2f{ subWeaponSignifierPosition.x + m_pHudVisuals[static_cast<int>(HudPartSprites::subWeaponTexture)]->GetCurrentFrameDimensions().width * 0.8f * scale, subWeaponSignifierPosition.y - 13.f }, false, false);
	}
}

void Hud::Update(float elapsedSec)
{
	m_CurrentLevelTime -= elapsedSec;
	if (m_CurrentLevelTime <= 0.f)
	{
		//m_pPlayer->Kill(Vector2f{});
	}

	Rectf
		newFrameDimension{ m_pHudVisuals[static_cast<int>(HudPartSprites::timerFill)]->GetCurrentFrameDimensions() };
	
	newFrameDimension.width = m_TimerFullWidth * (m_CurrentLevelTime / m_MaxLevelTime);

	m_pHudVisuals[static_cast<int>(HudPartSprites::timerFill)]->SetAnimationFrameInfo(newFrameDimension, 1, 1);

}
