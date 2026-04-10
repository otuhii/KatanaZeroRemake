#pragma once
#include "Sprite.h"

class Entity
{
public:
	Entity(Sprite* sprite, const Vector2f& position, const Vector2f& velocity, float speed);

	virtual void Draw() const;

	virtual void Update(const float elapsedSec, const Rectf& viewport);

	void SetSpeed(float speed);
	void SetVelocityX(float xVel);
	void SetVelocityY(float yVel);
	void SetVelocity(const Vector2f& velocity);

	void SetPositionX(float xPos);
	void SetPositionY(float yPos);
	void SetPosition(const Vector2f& position);

	void SetIsOnGroundState(bool state);
	void SetCanJumpThroughPlatform(bool canJump);

	void UpdateHitbox();

	bool			IsOnGround()	const;

	float			GetSpeed()		const;
	float			GetVelocityX()	const;
	float			GetVelocityY()	const;
	const Vector2f& GetVelocity()	const;


	float			GetPositionX()  const;
	float			GetPositionY()	const;
	const Vector2f& GetPosition()	const;

	const Rectf& GetHitbox()		const;

	bool			IsSpriteAnimationFinished() const;
	bool			CanJumpThroughPlatform() const;
protected:
	Sprite* GetSprite() const;

private:
	Sprite* m_pSprite;

	float m_Speed{};

	bool
		m_IsOnGround{},
		m_CanJumpThroughPlatform{};

	Vector2f m_Velocity{};
	Vector2f m_Position{};

	Rectf m_Hitbox{};
};

