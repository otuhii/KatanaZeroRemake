#include "pch.h"
#include "EnvironmentCosmeticObject.h"

EnvironmentCosmeticObject::EnvironmentCosmeticObject(float positionX, float positionY, const Sprite* pTexture)
	: m_Position(positionX, positionY), m_pTexture(pTexture)
{
}

const Vector2f& EnvironmentCosmeticObject::GetPosition() const
{
	return m_Position;
}

const Sprite* EnvironmentCosmeticObject::GetTexture() const
{
	return m_pTexture;
}

