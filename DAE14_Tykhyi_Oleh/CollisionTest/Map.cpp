#include "pch.h"
#include "Map.h"

#include "utils.h"

Map::Map(const std::vector<EnvironmentObject>& environmentObjects)
	: m_EnvironmentObjects(environmentObjects)
{
}

void Map::Draw() const
{
	utils::SetColor(Color4f{ 1.f, 1.f, 0.f, 1.f });

	for (const EnvironmentObject& obj : m_EnvironmentObjects)
	{
		for (const Rectf& collider : obj.GetColliders())
		{
			utils::FillRect(collider);
		}
	}
}

const std::vector<EnvironmentObject>& Map::GetEnvironmentObjects() const
{
	return m_EnvironmentObjects;
}
