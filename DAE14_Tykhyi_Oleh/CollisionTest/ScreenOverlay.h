#pragma once

class LevelManager;
class SpriteManager;
class Sprite;
class Hud;

class ScreenOverlay final
{
public:
	ScreenOverlay(SpriteManager* pSpriteManager, const Rectf& viewport, LevelManager* pLevelManager);
	~ScreenOverlay();

	void Draw() const;

	void Update(float elapsedSec, const Uint8* pStates);

private:
	Sprite*
		m_pRestartMessage{};
	Sprite*
		m_pReplayIcon{};

	Hud* 
		m_pHud{};

	LevelManager* 
		m_pLevelManager{};

	Rectf
		m_Viewport{};

	float
		m_TrackingTimer{ 0.f };

	void DrawRestartMessage() const;
	void DrawReplayOverlay() const;
};

