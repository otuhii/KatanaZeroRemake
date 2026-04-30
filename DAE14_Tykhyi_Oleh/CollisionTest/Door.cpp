#include "pch.h"
#include "Door.h"

#include "pch.h"
#include "Door.h"

Door::Door(
	Sprite* pSprite,
	const std::vector<AnimationFrameInfo>& doorAnimation,
	const Vector2f& position,
	int floor,
	const Rectf& openedHitbox,
	const Rectf& closedHitbox,
	float scale
)
	:
	InteractableObject{ position, floor, 0.f, InteractableObject::InteractableType::door },
	m_pSprite{ pSprite },
	m_SpriteFrames{ doorAnimation },
	m_OpenedHitbox{ openedHitbox },
	m_ClosedHitbox{ closedHitbox }
{
	m_pSprite->SetScale(scale);
	m_pSprite->SetAnimationFrameInfo(m_SpriteFrames[0]);
	m_pSprite->ResetAnimation();
	m_pSprite->SetStatic(true);
	m_pSprite->SetLooping(false);
	m_pSprite->FlipHorizontally();
}

void Door::Update(float elapsedSec)
{
	if (m_pSprite->IsFinished() && m_IsOpening)
	{
		m_IsOpening = false;
		m_IsOpened = true;
	}
}

void Door::Draw() const
{
	m_pSprite->Draw(GetPosition(), true, false);
}

void Door::Interact()
{
	m_pSprite->SetStatic(false);
	m_IsOpening = true;
}

bool Door::IsOpened() const
{
	return m_IsOpened;
}

bool Door::IsOpening() const
{
	return m_IsOpening;
}

const Rectf& Door::GetCurrentCollider() const
{
	if (m_IsOpened || m_IsOpening)
	{
		return m_OpenedHitbox;
	}
	else
	{
		return m_ClosedHitbox;
	}
}
