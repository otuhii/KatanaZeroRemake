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
	const std::vector<ControlPoint>& GetControlPoints()					      const;

	void SetEnvironmentActiveObjects(const std::vector<EnvironmentActiveObject>& activeObjects);
	void SetEnvironmentCosmeticObjects(const std::vector<EnvironmentCosmeticObject>& cosmeticObjects);
	void SetControlPoints(const std::vector<ControlPoint>& controlPoints);

private:
	std::vector<EnvironmentActiveObject>	m_EnvironmentActiveObjects{};
	std::vector<EnvironmentCosmeticObject>	m_EnvironmentCosmeticObjects{};
	std::vector<ControlPoint>				m_ControlPoints{};
};
