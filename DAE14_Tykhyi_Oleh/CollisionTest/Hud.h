#pragma once
#include <vector>

class SpriteManager;
class LevelManager;
class Sprite;
class Player;

class Hud final
{
public:
	explicit Hud(SpriteManager* pSpriteManager, const Rectf& viewport);

	Hud(const Hud&) = delete;
	Hud& operator=(const Hud&) = delete;
	Hud(Hud&&) = delete;
	Hud& operator=(Hud&&) = delete;

	void Draw(LevelManager* pLevelManager) const;

	void Update(float elapsedSec, const Uint8* pStates, LevelManager* pLevelManager);

private:
	enum class HudPartSprites {
		none=-1,
		
		hudBar,
		timerBox,
		timerFill,
		subWeaponTexture,
		battery,
		betteryFillPartCharged,
		betteryFillPartNotCharged,
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

	Layout
		m_Layout{};

	float
		m_TimerFillFullWidth{};

	bool
		m_IsShiftPressed{ false },
		m_IsSpacePressed{ false };

	Layout CalculateLayout(const Rectf& screenDimensions) const;

	void DrawPart(HudPartSprites type, const Vector2f& pos) const;

	void DrawTimer() const;
	void DrawBattery(LevelManager* pLevelManager) const;
	void DrawSubweaponPart(Player* pPlayer) const;
	void DrawBar() const;
};

