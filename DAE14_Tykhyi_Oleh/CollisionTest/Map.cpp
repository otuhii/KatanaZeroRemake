#include "pch.h"
#include "Map.h"

#include "utils.h"

#include "InteractableObject.h"

Map::~Map()
{
	for (InteractableObject* pObj : m_pInteractableObjects)
	{
		delete pObj;
	}
}

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


	for (const InteractableObject* pObj : m_pInteractableObjects)
	{
		pObj->Draw();
	}

}

void Map::Update(float elapsedSec)
{
	for (InteractableObject* pObj : m_pInteractableObjects)
	{
		pObj->Update(elapsedSec);
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

void Map::SetInteractableObjects(const std::vector<InteractableObject*> pInteractableObjects)
{
	m_pInteractableObjects = pInteractableObjects;
}

bool Map::AreSeparatedByActiveObject(const Vector2f& entityPosition1, const Vector2f& entityPosition2)
{
	for (const EnvironmentActiveObject& envObject : m_EnvironmentActiveObjects)
	{
		if (envObject.GetType() == EnvironmentActiveObject::EnvironmentObjectType::jumpThroughPlatform)
		{
			continue;
		}

		for (const Rectf& collider : envObject.GetColliders())
		{
			if (utils::IsOverlapping(entityPosition1, entityPosition2, collider))
			{
				return true;
			}
		}
	}

	return false;
}

InteractableObject* Map::GetClosestInteractableObject(const Vector2f& playerPos, int playerFloor) const
{
	for (InteractableObject* pObj : m_pInteractableObjects)
	{
		if (pObj->IsPlayerInRange(playerPos, playerFloor))
		{
			return pObj;
		}
	}
	return nullptr;
}

