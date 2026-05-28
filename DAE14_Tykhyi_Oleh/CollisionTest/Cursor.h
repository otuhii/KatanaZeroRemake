#pragma once

class Sprite;

class Cursor final
{
public:
	explicit Cursor(Sprite* pCursorSprite);

	Cursor(const Cursor&) = delete;
	Cursor& operator=(const Cursor&) = delete;
	Cursor(Cursor&&) = delete;
	Cursor& operator=(Cursor&&) = delete;

	void Draw() const;

	void Update(const SDL_MouseMotionEvent& event);

private:
	Sprite* m_pCursorSprite{};
	Vector2f m_Position{};
};

