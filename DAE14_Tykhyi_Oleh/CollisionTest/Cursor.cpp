#include "pch.h"
#include "Cursor.h"

#include "Sprite.h"

Cursor::Cursor(Sprite* pCursorSprite)
	: m_pCursorSprite{pCursorSprite}
{
	SDL_ShowCursor(SDL_DISABLE);
	m_pCursorSprite->SetScale(2.f);
}

void Cursor::Draw() const
{
	m_pCursorSprite->Draw(m_Position, true, true);
}

void Cursor::Update(const SDL_MouseMotionEvent& event)
{
	m_Position.x = static_cast<float>(event.x);
	m_Position.y = static_cast<float>(event.y);
}

const Vector2f& Cursor::GetPosition() const
{
	return m_Position;
}

