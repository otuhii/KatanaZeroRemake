#pragma once

class Sprite;


class Cursor final
{
public:
	Cursor(Sprite* pCursorSprite);

	Cursor(const Cursor&) = delete;
	Cursor& operator=(const Cursor&) = delete;

	void Draw() const;

	void Update(const SDL_MouseMotionEvent& event);

private:
	Sprite* m_pCursorSprite{};
	Vector2f m_Position{};
};

