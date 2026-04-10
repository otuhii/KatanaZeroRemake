#pragma once
#include <vector>
#include "EnvironmentActiveObject.h"
#include "EnvironmentCosmeticObject.h"

class Map final
{
public:
	void Draw() const;

	const std::vector<EnvironmentActiveObject>& GetEnvironmentActiveObjects() const;

	void SetEnvironmentActiveObjects(const std::vector<EnvironmentActiveObject>& activeObjects);
	void SetEnvironmentCosmeticObjects(const std::vector<EnvironmentCosmeticObject>& cosmeticObjects);

	void SetRespawnPoint(const Vector2f& respawnPoint);

	const Vector2f& GetRespawnPoint() const;
private:
	std::vector<EnvironmentActiveObject> m_EnvironmentActiveObjects{};
	std::vector<EnvironmentCosmeticObject> m_EnvironmentCosmeticObjects{};

	Vector2f m_RespawnPoint{};
};
