#include "pch.h"
#include "Entity.h"

Entity::Entity(Sprite* sprite, const Vector2f& position, const Vector2f& velocity, float speed)
	: m_pSprite{sprite}, m_Velocity(velocity), m_Speed(speed)
{

}

void Entity::Update(float elapsedSec)
{
	m_Position = m_Position + m_Velocity.Normalized() * elapsedSec * m_Speed;
}

void Entity::SetSpeed(float speed)
{
	m_Speed = speed;
}

void Entity::SetVelocity(const Vector2f& velocity)
{
	m_Velocity = velocity;
}

void Entity::SetPosition(const Vector2f& position)
{
	m_Position = position;
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

const Vector2f& Entity::GetPosition() const
{
	return m_Position;
}

Sprite* Entity::GetSprite() const
{
	return m_pSprite;
}
