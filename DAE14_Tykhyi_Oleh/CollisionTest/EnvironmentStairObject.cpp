#include "pch.h"
#include "EnvironmentStairObject.h"


EnvironmentStairObject::EnvironmentStairObject(
	float positionX, 
	float positionY,
	const Rectf& firstCollider,
	const Rectf& secondCollider,
	const std::string& texturePath
)
	: EnvironmentBaseObject(positionX, positionY, firstCollider, texturePath), m_SecondCollider(secondCollider)
{
	SetType(EnvironmentObjectType::stairs);
}

const Rectf& EnvironmentStairObject::GetSecondCollider() const
{
	return m_SecondCollider;
}
