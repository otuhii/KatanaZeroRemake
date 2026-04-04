#include "pch.h"
#include "EnvironmentObject.h"


EnvironmentObject::EnvironmentObject(
	float positionX,
	float positionY,
	const std::vector<Rectf>& colliders,
	const std::string& texturePath
)
	: m_Colliders(colliders),
	m_Position(positionX, positionY),
	m_TexturePath(texturePath)
{
}

const std::vector<Rectf>& EnvironmentObject::GetColliders() const
{
	return m_Colliders;
}


const Vector2f& EnvironmentObject::GetPosition() const
{
	return m_Position;
}
