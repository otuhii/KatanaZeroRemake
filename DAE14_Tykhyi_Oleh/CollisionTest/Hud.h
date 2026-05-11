#pragma once

#include <vector>

class SpriteManager;
class Sprite;
class Player;

class Hud final
{
public:
	Hud(Player* pPlayer, SpriteManager* pSpriteManager);

	void Draw(const Rectf& screenDimensions) const;

	void Update(float elapsedSec);

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

	std::vector<Sprite*> m_pHudVisuals{};
	Player* m_pPlayer;

	const float
		m_MaxSlowTime{4.f},
		m_MaxLevelTime{15.f};


	float
		m_CurrentSlowTime{},
		m_CurrentLevelTime{m_MaxLevelTime};

	bool
		m_IsSlowmo{};

	bool
		m_IsShiftPressed{ false },
		m_IsSpacePressed{ false };

	float
		m_TimerFullWidth{};
};

