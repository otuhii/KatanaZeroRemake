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

Sprite* SpriteManager::CreateSprite(const std::string& path) 
{
	Sprite* pNewSprite = new Sprite{ path };

	m_pSprites.push_back(pNewSprite);

	return pNewSprite;
}

//TODO consider that after deletion of the sprite, my player or enemy instance still have that address, maybe i should consider adding some check when it calls
void SpriteManager::RemoveSprite(int itemIndex) 
{
	if (itemIndex < 0 || itemIndex >= m_pSprites.size())
	{
		return;
	}

	delete m_pSprites[itemIndex];

	m_pSprites[itemIndex] = m_pSprites.back();

	m_pSprites.pop_back();
}
