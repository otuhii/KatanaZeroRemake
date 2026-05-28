#pragma once
#include <vector>
#include <string>

class Sprite;

class SpriteManager final
{
public:
	SpriteManager() = default;
	~SpriteManager();

	SpriteManager(const SpriteManager&) = delete;
	SpriteManager& operator=(const SpriteManager&) = delete;
	SpriteManager(SpriteManager&&) = delete;
	SpriteManager& operator=(SpriteManager&&) = delete;

	void Update(float elapsedSec);

	Sprite* GetSprite(int index) const;

	Sprite* CreateSprite(const std::string& path);
	Sprite* CreateEmptySprite();
	Sprite* CreateCopySprite(const Sprite* pOriginal);
	
	void RemoveSprite(int index);
private:
	std::vector<Sprite*> m_pSprites;
};

