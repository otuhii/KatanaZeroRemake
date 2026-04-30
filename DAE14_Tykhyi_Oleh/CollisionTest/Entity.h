#pragma once
#include "Sprite.h"

class Entity
{
public:
	enum class EntityType {
		player,
		enemy
	};

	Entity(Sprite* sprite, EntityType type, const Vector2f& position, const Vector2f& velocity, float speed, int floor);

	virtual void Draw() const;

	virtual void Update(const float elapsedSec, const Rectf& viewport);

	virtual ~Entity() = default;

	void SetFloor(int floor);

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

	int				GetFloor()		const;

	float			GetSpeed()		const;
	float			GetVelocityX()	const;
	float			GetVelocityY()	const;
	const Vector2f& GetVelocity()	const;


	float			GetPositionX()  const;
	float			GetPositionY()	const;
	const Vector2f& GetPosition()	const;

	EntityType		GetType()		const;

	const Rectf& GetCurrentHitbox()		const;
	const Rectf& GetPreviousHitbox() const;

	int				GetFacingDirection() const;
	bool			IsSpriteAnimationFinished() const;
	bool			CanJumpThroughPlatform() const;

	virtual void	Kill(const Vector2f& impulse);
	bool			IsAlive() const;
protected:
	Sprite* GetSprite() const;

private:
	Sprite* m_pSprite;

	EntityType 
		m_Type{};

	float
		m_Speed{};

	int
		m_Floor{};

	bool
		m_IsOnGround{},
		m_CanJumpThroughPlatform{},
		m_IsAlive{ true };

	Vector2f m_Velocity{};
	Vector2f m_Position{};

	Rectf
		m_CurrentHitbox{},
		m_PreviousHitbox{};

	void StoreHitbox();
};

