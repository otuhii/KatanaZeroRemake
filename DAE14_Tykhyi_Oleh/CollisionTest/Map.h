#pragma once
#include <vector>
#include "EnvironmentActiveObject.h"
#include "EnvironmentCosmeticObject.h"

class Map final
{
public:
	void Draw() const;

	const std::vector<EnvironmentActiveObject>& GetEnvironmentActiveObjects() const;
	const std::vector<Vector2f>& GetControlPoints()							  const;
	const Vector2f&				 GetRespawnPoint()							  const;


	void SetEnvironmentActiveObjects(const std::vector<EnvironmentActiveObject>& activeObjects);
	void SetEnvironmentCosmeticObjects(const std::vector<EnvironmentCosmeticObject>& cosmeticObjects);
	void SetControlPoints(const std::vector<Vector2f>& controlPoints);

	void SetRespawnPoint(const Vector2f& respawnPoint);


private:
	std::vector<EnvironmentActiveObject>	m_EnvironmentActiveObjects{};
	std::vector<EnvironmentCosmeticObject>	m_EnvironmentCosmeticObjects{};
	std::vector<Vector2f>					m_ControlPoints{};

	Vector2f m_RespawnPoint{};
};
