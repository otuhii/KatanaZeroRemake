#include "pch.h"
#include "Map.h"

#include "utils.h"

void Map::Draw() const
{
	utils::SetColor(Color4f{ 1.f, 1.f, 0.f, 1.f });

	for (const EnvironmentActiveObject& obj : m_EnvironmentActiveObjects)
	{
		if (obj.GetTexture() != nullptr)
		{
			obj.GetTexture()->Draw(obj.GetPosition(), false, false);
		}

		utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
		for (const Rectf& collider : obj.GetColliders())
		{
			//utils::DrawRect(collider);
		}
	}

	for (const EnvironmentCosmeticObject& obj : m_EnvironmentCosmeticObjects)
	{
		if (obj.GetTexture() != nullptr)
		{
			obj.GetTexture()->Draw(obj.GetPosition(), false, false);
		}
	}
}

const std::vector<EnvironmentActiveObject>& Map::GetEnvironmentActiveObjects() const
{
	return m_EnvironmentActiveObjects;
}

void Map::SetEnvironmentActiveObjects(const std::vector<EnvironmentActiveObject>& activeObjects)
{
	m_EnvironmentActiveObjects = activeObjects;
}

void Map::SetEnvironmentCosmeticObjects(const std::vector<EnvironmentCosmeticObject>& cosmeticObjects) 
{
	m_EnvironmentCosmeticObjects = cosmeticObjects;
}


