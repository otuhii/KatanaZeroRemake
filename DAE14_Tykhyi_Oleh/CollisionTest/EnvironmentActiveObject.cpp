#include "pch.h"
#include "EnvironmentActiveObject.h"


EnvironmentActiveObject::EnvironmentActiveObject(
	float positionX,
	float positionY,
	const std::vector<Rectf>& colliders,
	const Sprite* pTexture,
	EnvironmentObjectType type
)
	: m_Colliders(colliders),
	m_Position(positionX, positionY),
	m_pTexture(pTexture),
	m_Type{type}
{
}

const std::vector<Rectf>& EnvironmentActiveObject::GetColliders() const
{
	return m_Colliders;
}


const Vector2f& EnvironmentActiveObject::GetPosition() const
{
	return m_Position;
}

const Sprite* EnvironmentActiveObject::GetTexture() const
{
	return m_pTexture;
}

EnvironmentActiveObject::EnvironmentObjectType EnvironmentActiveObject::GetType() const
{
	return m_Type;
}
