#pragma once

class LevelManager;
class SpriteManager;
class Sprite;

class ScreenOverlay final
{
public:
	ScreenOverlay(SpriteManager* pSpriteManager);

	void Draw(const LevelManager* pLevelManager, const Rectf& viewport) const;
private:
	Sprite*
		m_pRestartMessage{};

	void DrawRestartMessage(const Rectf& viewport) const;
};

