#include "pch.h"
#include "EnvironmentBaseObject.h"

EnvironmentBaseObject::EnvironmentBaseObject(
	float positionX, 
	float positionY,
	const Rectf& firstCollider,
	const std::string& texturePath
)
	: m_FirstCollider(firstCollider),
	m_Position(positionX, positionY),
	m_TexturePath(texturePath), 
	m_Type(EnvironmentObjectType::base)
{}

EnvironmentBaseObject::EnvironmentObjectType EnvironmentBaseObject::GetType() const
{
	return m_Type;
}

const Rectf& EnvironmentBaseObject::GetFirstCollider() const
{
	return m_FirstCollider;
}

void EnvironmentBaseObject::SetType(EnvironmentObjectType type)
{
	m_Type = type;
}
