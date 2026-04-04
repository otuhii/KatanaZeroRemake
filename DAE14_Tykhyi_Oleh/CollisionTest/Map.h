#pragma once
#include <vector>
#include "EnvironmentObject.h"

class Map final
{
public:
	Map(const std::vector<EnvironmentObject>& environmentObjects);

	void Draw() const;

	const std::vector<EnvironmentObject>& GetEnvironmentObjects() const;

private:
	std::vector<EnvironmentObject> m_EnvironmentObjects{};
};
