#include "pch.h"
#include "Entity.h"

#include "CollisionManager.h"

#include "utils.h"
#include "AttackParticle.h"

Entity::Entity(Sprite* sprite, EntityType type, const Vector2f& position, const Vector2f& velocity, float speed, int floor)
	: m_pSprite(sprite), m_Velocity(velocity), m_Speed(speed), m_IsOnGround(true), m_Position{position}, m_Floor{floor}, m_Type{type}, m_InitialPosition{position}, m_InitialFloor{floor}
{
}

void Entity::Draw() const
{
	m_pSprite->Draw(m_Position, true, false);

	utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	//utils::DrawRect(GetCurrentHitbox());
}

void Entity::Update(float elapsedSec, const Rectf& viewport)
{
	float
		gravity{ -981.f },
		frictionMultiplier{ 0.95f };

	m_Velocity.y += gravity * elapsedSec;

	if (!IsAlive())
	{
		m_Velocity.x *= frictionMultiplier;
	}
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
	StoreHitbox();

	const float
		hitboxWidth{ 20.f },
		hitboxHeight{ 30.f };

	m_CurrentHitbox.width = hitboxWidth * GetSprite()->GetScale();
	m_CurrentHitbox.height = hitboxHeight * GetSprite()->GetScale();

	m_CurrentHitbox.left = m_Position.x - (m_CurrentHitbox.width * 0.5f);
	m_CurrentHitbox.bottom = m_Position.y;
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

Entity::EntityType Entity::GetType() const
{
	return m_Type;
}

void Entity::StoreHitbox() 
{
	m_PreviousHitbox = m_CurrentHitbox;
}

const Rectf& Entity::GetCurrentHitbox() const
{
	return m_CurrentHitbox;
}

const Rectf& Entity::GetPreviousHitbox() const
{
	return m_PreviousHitbox;
}

int Entity::GetCurrentFrame() const
{
	return m_pSprite->GetCurrentFrameCount();
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

void Entity::Kill(const Vector2f& impulse) 
{
	m_IsAlive = false;
	GetSprite()->SetLooping(false);
	SetVelocity(impulse);
}

bool Entity::IsAlive() const
{
	return m_IsAlive;
}

void Entity::Reset() 
{
	GetSprite()->SetLooping(true);
	m_Position = m_InitialPosition;
	m_Velocity = Vector2f{};
	m_IsAlive = true;
	m_Floor = m_InitialFloor;
	m_CanJumpThroughPlatform = false;
}

Sprite* Entity::GetSprite() const
{
	return m_pSprite;
}
