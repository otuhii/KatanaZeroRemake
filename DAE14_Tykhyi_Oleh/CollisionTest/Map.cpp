#include "pch.h"
#include "Map.h"

#include "utils.h"

#include "InteractableObject.h"
#include "Door.h"

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
	
	for (const EnvironmentCosmeticObject& obj : m_EnvironmentCosmeticObjects)
	{
		if (obj.GetTexture() != nullptr)
		{
			obj.GetTexture()->Draw(obj.GetPosition(), false, false);
		}
	}

	for (const EnvironmentActiveObject& obj : m_EnvironmentActiveObjects)
	{
		if (obj.GetTexture() != nullptr)
		{
			obj.GetTexture()->Draw(obj.GetPosition(), false, false);
		}

		utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
		//for (const Rectf& collider : obj.GetColliders())
		//{
		//	//utils::DrawRect(collider);
		//}
	}

	for (const InteractableObject* pObj : m_pInteractableObjects)
	{
		if (pObj->IsActive())
		{
			pObj->Draw();
			if (pObj->GetType() == InteractableObject::InteractableType::door)
			{
				const Door* pDoor = static_cast<const Door*>(pObj);
				utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
				//utils::DrawRect(pDoor->GetCurrentCollider());
			}
		}
	}

}

void Map::Update(float elapsedSec, SoundManager* pSoundManager, Player* pPlayer)
{
	for (InteractableObject* pObj : m_pInteractableObjects)
	{
		pObj->Update(elapsedSec, pSoundManager, pPlayer);
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

void Map::SetInteractableObjects(const std::vector<InteractableObject*>& pInteractableObjects)
{
	m_pInteractableObjects = pInteractableObjects;
}

bool Map::AreSeparatedByActiveObject(const Vector2f& entityPosition1, const Vector2f& entityPosition2) const
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

	for (const InteractableObject* pInteractableObject : m_pInteractableObjects)
	{
		if (pInteractableObject->GetType() == InteractableObject::InteractableType::door)
		{
			const Door* pDoor = static_cast<const Door*>(pInteractableObject);
			if (!pDoor->IsOpened() && !pDoor->IsOpening())
			{
				if (utils::IsOverlapping(entityPosition1, entityPosition2, pDoor->GetCurrentCollider()))
				{
					return true;
				}
			}
		}
	}

	return false;
}

const std::vector<InteractableObject*>& Map::GetInteractableObjects() const
{
	return m_pInteractableObjects;
}

InteractableObject* Map::GetClosestInteractableObject(const Vector2f& playerPos, int playerFloor) const
{
	for (InteractableObject* pObject : m_pInteractableObjects)
	{
		if (pObject->IsPlayerInRange(playerPos, playerFloor) && pObject->IsActive())
		{
			return pObject;
		}
	}
	return nullptr;
}

void Map::Reset() const
{
	for (InteractableObject* pObj : m_pInteractableObjects)
	{
		pObj->Reset();
	}
}

