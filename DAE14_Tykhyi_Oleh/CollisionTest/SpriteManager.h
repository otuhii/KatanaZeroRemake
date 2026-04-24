#pragma once
#include <vector>

class Sprite;

class SpriteManager final
{
public:

	~SpriteManager();

	void Update(float elapsedSec);

	Sprite* GetSprite(int index) const;

	Sprite* CreateSprite(const std::string& path);

	Sprite* CloneSprite(const Sprite* pOriginal);

	void RemoveSprite(int index);

private:
	std::vector<Sprite*> m_pSprites;
};

