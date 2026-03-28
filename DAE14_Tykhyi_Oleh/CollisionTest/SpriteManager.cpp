#include "pch.h"
#include "SpriteManager.h"
#include "Sprite.h"

SpriteManager::~SpriteManager()
{
	for (const Sprite* pSprite : m_pSprites)
	{
		delete pSprite;
	}
}

void SpriteManager::Update(float elapsedSec)
{
	for (Sprite* pSprite : m_pSprites)
	{
		pSprite->Update(elapsedSec);
	}
}

Sprite* SpriteManager::GetSprite(int index) const
{
	return m_pSprites.at(index);
}

void SpriteManager::AddSprite(int rows, int cols, const std::string& path) // bad indexing
{
	m_pSprites.push_back(new Sprite{ rows, cols, path });
}



void SpriteManager::RemoveSprite(int itemIndex) //TODO better deletion and addition of elements
{
	if (m_pSprites.at(itemIndex) != nullptr)
	{
		delete m_pSprites[itemIndex];
		m_pSprites[itemIndex] = nullptr;
	}

}
