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
	Sprite* CreateEmptySprite();
	Sprite* CreateCopySprite(const Sprite* pOriginal);
	
	//void CloneSprite(Sprite* pDestination, Sprite* pSource);

	void RemoveSprite(int index);

private:
	std::vector<Sprite*> m_pSprites;
};

