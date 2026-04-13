#include "pch.h"
#include "Entity.h"

#include "CollisionManager.h"

#include "utils.h"

Entity::Entity(Sprite* sprite, const Vector2f& position, const Vector2f& velocity, float speed, int floor)
	: m_pSprite(sprite), m_Velocity(velocity), m_Speed(speed), m_IsOnGround(true), m_Position{position}, m_Floor{floor}
{
}

void Entity::Draw() const
{
	m_pSprite->Draw(m_Position, true, false);

	utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	utils::DrawRect(GetHitbox());
}

void Entity::Update(float elapsedSec, const Rectf& viewport)
{
	float 
		gravity{ -981.f };

	m_Velocity.y += gravity * elapsedSec;

}

void Entity::SetFloor(int floor)
{
	m_Floor = floor;
}

void Entity::SetSpeed(float speed)
{
	m_Speed = speed;
}

void Entity::SetVelocity(const Vector2f& velocity)
{
	m_Velocity = velocity;
}

void Entity::SetPositionX(float xPos)
{
	m_Position.x = xPos;
	UpdateHitbox();
}

void Entity::SetPositionY(float yPos)
{
	m_Position.y = yPos;
	UpdateHitbox();
}

void Entity::SetPosition(const Vector2f& position)
{
	m_Position = position;
	UpdateHitbox();
}

void Entity::SetIsOnGroundState(bool state)
{
	m_IsOnGround = state;
}

void Entity::SetCanJumpThroughPlatform(bool canJump)
{
	m_CanJumpThroughPlatform = canJump;
}

void Entity::UpdateHitbox()
{
	const float
		hitboxWidth{ 20.f },
		hitboxHeight{ 30.f };

	m_Hitbox.width = hitboxWidth * GetSprite()->GetScale();
	m_Hitbox.height = hitboxHeight * GetSprite()->GetScale();

	m_Hitbox.left = m_Position.x - (m_Hitbox.width * 0.5f);
	m_Hitbox.bottom = m_Position.y;
}

bool Entity::IsOnGround() const
{
	return m_IsOnGround;
}

int Entity::GetFloor() const
{
	return m_Floor;
}

void Entity::SetVelocityX(float xVel)
{
	m_Velocity.x = xVel;
}

void Entity::SetVelocityY(float yVel)
{
	m_Velocity.y = yVel;
}

float Entity::GetSpeed() const
{
	return m_Speed;
}

float Entity::GetVelocityX() const
{
	return m_Velocity.x;
}

float Entity::GetVelocityY() const
{
	return m_Velocity.y;
}

const Vector2f& Entity::GetVelocity() const
{
	return m_Velocity;
}

float Entity::GetPositionX() const
{
	return m_Position.x;
}

float Entity::GetPositionY() const
{
	return m_Position.y;
}

const Vector2f& Entity::GetPosition() const
{
	return m_Position;
}

const Rectf& Entity::GetHitbox() const
{
	return m_Hitbox;
}

int Entity::GetFacingDirection() const
{
	if (m_pSprite->IsFlippedHorizontally())
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

bool Entity::IsSpriteAnimationFinished() const
{
	return m_pSprite->IsFinished();
}

bool Entity::CanJumpThroughPlatform() const
{
	return m_CanJumpThroughPlatform;
}

void Entity::Kill() 
{
	m_IsAlive = false;
}

bool Entity::IsAlive() const
{
	return m_IsAlive;
}

Sprite* Entity::GetSprite() const
{
	return m_pSprite;
}
