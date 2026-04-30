#pragma once
#include <vector>
#include "EnvironmentActiveObject.h"
#include "EnvironmentCosmeticObject.h"
#include "ControlPoint.h"

class InteractableObject;

class Map final
{
public:
	~Map();

	void Draw() const;

	void Update(float elapsedSec);

	const std::vector<EnvironmentActiveObject>& GetEnvironmentActiveObjects() const;

	void SetEnvironmentActiveObjects(const std::vector<EnvironmentActiveObject>& activeObjects);
	void SetEnvironmentCosmeticObjects(const std::vector<EnvironmentCosmeticObject>& cosmeticObjects);
	void SetInteractableObjects(const std::vector<InteractableObject*> pInteractableObjects);

	bool AreSeparatedByActiveObject(const Vector2f& entityPosition1, const Vector2f& entityPosition2);

	InteractableObject* GetClosestInteractableObject(const Vector2f& playerPos, int playerFloor) const;
private:
	std::vector<EnvironmentActiveObject>	m_EnvironmentActiveObjects{};
	std::vector<EnvironmentCosmeticObject>	m_EnvironmentCosmeticObjects{};
	std::vector<InteractableObject*>		m_pInteractableObjects{};
};
