#include "pch.h"
#include "ThrowableObject.h"

ThrowableObject::ThrowableObject(Sprite* pSprite, const Vector2f& position, float interactionRange, float scale, int floor)
	:InteractableObject{ position, floor, interactionRange, InteractableType::throwableObject },
	m_pSprite{ pSprite }
{
	m_pSprite->SetScale(scale);
}

void ThrowableObject::Draw() const
{
	m_pSprite->Draw(GetPosition(), true, false);
}

void ThrowableObject::Interact(SoundManager* pSoundManager)
{
	SetInactive();
}

Sprite* ThrowableObject::GetSprite() const
{
	return m_pSprite;
}