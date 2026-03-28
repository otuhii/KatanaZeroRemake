#pragma once
#include "Sprite.h"


class Entity
{
public:
	Entity(Sprite* sprite, const Vector2f& position, const Vector2f& velocity, float speed);

	virtual void Draw() const = 0;

	virtual void Update(const float elapsedSec);

	void SetSpeed(float speed);
	void SetVelocityX(float xVel);
	void SetVelocityY(float yVel);
	void SetVelocity(const Vector2f& velocity);

	void SetPosition(const Vector2f& position);

	float			GetSpeed()		const;
	float			GetVelocityX()	const;
	float			GetVelocityY()	const;
	const Vector2f& GetVelocity()	const;

	const Vector2f& GetPosition()	const;

protected:
	Sprite* GetSprite() const;

private:
	Sprite* m_pSprite;

	float m_Speed{};

	Vector2f m_Velocity{};
	Vector2f m_Position{};
};

