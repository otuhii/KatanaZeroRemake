#pragma once
#include <vector>
#include "EnvironmentActiveObject.h"
#include "EnvironmentCosmeticObject.h"
#include "ControlPoint.h"

class Map final
{
public:
	void Draw() const;

	const std::vector<EnvironmentActiveObject>& GetEnvironmentActiveObjects() const;

	void SetEnvironmentActiveObjects(const std::vector<EnvironmentActiveObject>& activeObjects);
	void SetEnvironmentCosmeticObjects(const std::vector<EnvironmentCosmeticObject>& cosmeticObjects);

private:
	std::vector<EnvironmentActiveObject>	m_EnvironmentActiveObjects{};
	std::vector<EnvironmentCosmeticObject>	m_EnvironmentCosmeticObjects{};
};
