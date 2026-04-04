#include "pch.h"
#include "Map.h"

#include "utils.h"

Map::Map(const std::vector<EnvironmentObject>& environmentObjects)
	: m_EnvironmentObjects(environmentObjects)
{
}

void Map::Draw() const
{
	
}

const std::vector<EnvironmentObject>& Map::GetEnvironmentObjects() const
{
	return m_EnvironmentObjects;
}
