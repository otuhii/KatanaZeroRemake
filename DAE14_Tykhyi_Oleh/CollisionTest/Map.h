#pragma once
#include <vector>
#include "EnvironmentBaseObject.h"
#include "EnvironmentStairObject.h"

class Map final
{
public:
	Map(const std::vector<EnvironmentBaseObject*>& environmentObjects);

	~Map();

	void Draw() const;

	const std::vector<EnvironmentBaseObject*>& GetEnvironmentObjects() const;

private:
	std::vector<EnvironmentBaseObject*> m_EnvironmentObjects{};
};
