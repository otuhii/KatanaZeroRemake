#pragma once

#include <vector>

class SpriteManager;
class Sprite;
class Player;

class Hud final
{
public:
	Hud(const Rectf& viewport, Player* pPlayer, SpriteManager* pSpriteManager);

	void Draw() const;

	void Update(float elapsedSec, const Uint8* pStates);

private:
	enum class HudPartSprites {
		none=-1,
		
		hudBar,
		timerBox,
		timerFill,
		subWeaponTexture,
		battery,
		betteryFillPart,
		spacePressed,
		spaceReleased,
		shiftPressed,
		shiftReleased,
		leftClick,
		katanaIcon,
		pickedItem,

		count
	};

	struct Layout {
		Vector2f basePos{};
		Vector2f timerPos{};
		Vector2f timerFillPos{};
		Vector2f batteryPos{};
		Vector2f batterFillPos{};
		Vector2f batteryFillOffset{};
		Vector2f subweaponPartPos{};
		Vector2f subweaponKatanaPos{};
		Vector2f subweaponPickedPos{};
		Vector2f leftSubweaponButtonIconPos{};
		Vector2f rightSubweaponButtonIconPos{};
		Vector2f batteryButtonIconPos{};

		float scale{};
	};

	std::vector<Sprite*> m_pHudVisuals{};
	Player* m_pPlayer;

	Layout
		m_Layout{};

	const float
		m_MaxSlowTime{4.f},
		m_MaxLevelTime{15.f};


	float
		m_CurrentSlowTime{},
		m_CurrentLevelTime{m_MaxLevelTime},
		m_TimerFillFullWidth{};

	bool
		m_IsSlowmo{},
		m_IsShiftPressed{ false },
		m_IsSpacePressed{ false };

	Layout CalculateLayout(const Rectf& screenDimensions) const;

	void DrawPart(HudPartSprites type, const Vector2f& pos) const;

	void DrawTimer() const;
	void DrawBattery() const;
	void DrawSubweaponPart() const;
	void DrawBar() const;
};

