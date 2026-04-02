#include "pch.h"
#include "Map.h"

#include "utils.h"

Map::Map(const std::vector<EnvironmentBaseObject*>& environmentObjects)
	: m_EnvironmentObjects(environmentObjects)
{
}

void Map::Draw() const
{
	for (EnvironmentBaseObject* pEnvObj : m_EnvironmentObjects)
	{
		utils::SetColor(Color4f{ 1.f, 1.f, 0.f, 1.f });
		utils::FillRect(pEnvObj->GetFirstCollider());

		if (pEnvObj->GetType() == EnvironmentBaseObject::EnvironmentObjectType::stairs)
		{
			utils::FillRect(dynamic_cast<EnvironmentStairObject*>(pEnvObj)->GetSecondCollider());
		}
	}
}

Map::~Map()
{
	for (EnvironmentBaseObject* pEnvObj : m_EnvironmentObjects)
	{
		delete pEnvObj;
	}
}

const std::vector<EnvironmentBaseObject*>& Map::GetEnvironmentObjects() const
{
	return m_EnvironmentObjects;
}
