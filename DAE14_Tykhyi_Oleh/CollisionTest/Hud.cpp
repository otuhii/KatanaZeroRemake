#include "pch.h"
#include "Hud.h"

#include "SpriteManager.h"
#include "Player.h"
#include "ThrowableObject.h"


Hud::Hud(const Rectf& viewport, Player* pPlayer, SpriteManager* pSpriteManager)
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

	m_TimerFillFullWidth = m_pHudVisuals[static_cast<int>(HudPartSprites::timerFill)]->GetCurrentFrameDimensions().width;

	m_Layout = CalculateLayout(viewport);
}

void Hud::Draw() const
{
	DrawBar();
	DrawTimer();
	DrawBattery();
	DrawSubweaponPart();
}

void Hud::Update(float elapsedSec, const Uint8* pStates)
{
	m_CurrentLevelTime -= elapsedSec;
	if (m_CurrentLevelTime <= 0.f)
	{
		//m_pPlayer->Kill(Vector2f{});
	}

	m_IsSpacePressed = static_cast<bool>(pStates[SDL_SCANCODE_SPACE]);
	m_IsShiftPressed = static_cast<bool>(pStates[SDL_SCANCODE_LSHIFT]);

	Rectf
		newFrameDimension{ m_pHudVisuals[static_cast<int>(HudPartSprites::timerFill)]->GetCurrentFrameDimensions() };
	
	newFrameDimension.width = m_TimerFillFullWidth * (m_CurrentLevelTime / m_MaxLevelTime);

	m_pHudVisuals[static_cast<int>(HudPartSprites::timerFill)]->SetAnimationFrameInfo(newFrameDimension, 1, 1);

}

Hud::Layout Hud::CalculateLayout(const Rectf& screenDimensions) const
{
	Layout layout{};

	float scale{
		m_pHudVisuals[static_cast<int>(HudPartSprites::battery)]->GetScale()
	};

	Vector2f barDimensions{
		m_pHudVisuals[static_cast<int>(HudPartSprites::hudBar)]->GetCurrentFrameDimensions().width* scale,
		m_pHudVisuals[static_cast<int>(HudPartSprites::hudBar)]->GetCurrentFrameDimensions().height * scale
	};

	layout.basePos = Vector2f{
		0.f, screenDimensions.height - barDimensions.y
	};

	layout.timerPos = Vector2f{
		layout.basePos.x + barDimensions.x * 0.5f - m_pHudVisuals[static_cast<int>(HudPartSprites::timerBox)]->GetCurrentFrameDimensions().width * scale * 0.5f,
		layout.basePos.y
	};

	Vector2f timerFillOffset{ 34.f, 12.f };
	layout.timerFillPos = Vector2f{
		layout.timerPos.x + timerFillOffset.x,
		layout.basePos.y + timerFillOffset.y
	};

	layout.batteryPos = Vector2f{
		0.f,
		screenDimensions.height - barDimensions.y
	};

	layout.batteryFillOffset = Vector2f{ 
		10.f,
		0.f 
	};

	layout.batterFillPos = Vector2f{
		layout.batteryPos.x + 22.f,
		layout.batteryPos.y + 10.f
	};

	layout.batteryButtonIconPos = Vector2f{
		layout.batteryPos.x + m_pHudVisuals[static_cast<int>(HudPartSprites::battery)]->GetCurrentFrameDimensions().width * scale,
		layout.batteryPos.y + 7.f
	};


	Vector2f subWeaponSignifierPosition{
		screenDimensions.width - m_pHudVisuals[static_cast<int>(HudPartSprites::subWeaponTexture)]->GetCurrentFrameDimensions().width * scale - 25.f,
		screenDimensions.height - barDimensions.y
	};

	layout.subweaponPartPos = subWeaponSignifierPosition;

	layout.subweaponKatanaPos = subWeaponSignifierPosition;

	layout.subweaponPickedPos = Vector2f{
		subWeaponSignifierPosition.x + m_pHudVisuals[static_cast<int>(HudPartSprites::subWeaponTexture)]->GetCurrentFrameDimensions().width * 0.6f * scale,
		subWeaponSignifierPosition.y
	};

	layout.leftSubweaponButtonIconPos = Vector2f{
		subWeaponSignifierPosition.x + m_pHudVisuals[static_cast<int>(HudPartSprites::subWeaponTexture)]->GetCurrentFrameDimensions().width * 0.35f * scale,
		subWeaponSignifierPosition.y - 15.f
	};

	layout.rightSubweaponButtonIconPos = Vector2f{
		subWeaponSignifierPosition.x + m_pHudVisuals[static_cast<int>(HudPartSprites::subWeaponTexture)]->GetCurrentFrameDimensions().width * 0.8f * scale,
		subWeaponSignifierPosition.y - 15.f
	};
	
	return layout;
}


void Hud::DrawPart(HudPartSprites type, const Vector2f& pos) const
{
	m_pHudVisuals[static_cast<int>(type)]->Draw(pos, false, false);
}

void Hud::DrawTimer() const
{
	DrawPart(HudPartSprites::timerBox, m_Layout.timerPos);
	DrawPart(HudPartSprites::timerFill, m_Layout.timerFillPos);
}

void Hud::DrawBattery() const
{
	DrawPart(HudPartSprites::battery, m_Layout.batteryPos);
	if (m_IsShiftPressed)
	{
		DrawPart(HudPartSprites::shiftPressed, m_Layout.batteryButtonIconPos);
	}
	else
	{
		DrawPart(HudPartSprites::shiftReleased, m_Layout.batteryButtonIconPos);
	}
	
	int activeCells{ static_cast<int>(11 ) };
	Vector2f cellPos{ m_Layout.batterFillPos };
	for (int counter{ 0 }; counter < activeCells; ++counter)
	{
		DrawPart(HudPartSprites::betteryFillPart, cellPos);
		cellPos += m_Layout.batteryFillOffset;
	}

}

void Hud::DrawSubweaponPart() const
{
	DrawPart(HudPartSprites::subWeaponTexture, m_Layout.subweaponPartPos);
	DrawPart(HudPartSprites::katanaIcon, m_Layout.subweaponKatanaPos);
	DrawPart(HudPartSprites::pickedItem, m_Layout.subweaponPickedPos);

	if (m_pPlayer->GetHeldObject())
	{
		m_pPlayer->GetHeldObject()->GetSprite()->Draw(m_Layout.subweaponPickedPos, false, false);
	}

	DrawPart(HudPartSprites::leftClick, m_Layout.leftSubweaponButtonIconPos);

	if (m_IsSpacePressed)
	{
		DrawPart(HudPartSprites::spacePressed, m_Layout.rightSubweaponButtonIconPos);
	}
	else
	{
		DrawPart(HudPartSprites::spaceReleased, m_Layout.rightSubweaponButtonIconPos);
	}
}

void Hud::DrawBar() const
{
	DrawPart(HudPartSprites::hudBar, m_Layout.basePos);
}
