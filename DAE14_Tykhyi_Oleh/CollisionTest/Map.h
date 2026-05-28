#pragma once
#include <vector>
#include "EnvironmentActiveObject.h"
#include "EnvironmentCosmeticObject.h"

class InteractableObject;
class SoundManager;
class Player;

class Map final
{
public:
	Map() = default;

	~Map();

	Map(const Map&) = delete;
	Map& operator=(const Map&) = delete;
	Map(Map&&) = delete;
	Map& operator=(Map&&) = delete;

	void Draw() const;

	void Update(float elapsedSec, SoundManager* pSoundManager, Player* pPlayer);

	const std::vector<EnvironmentActiveObject>& GetEnvironmentActiveObjects() const;

	void SetEnvironmentActiveObjects(const std::vector<EnvironmentActiveObject>& activeObjects);
	void SetEnvironmentCosmeticObjects(const std::vector<EnvironmentCosmeticObject>& cosmeticObjects);
	void SetInteractableObjects(const std::vector<InteractableObject*>& pInteractableObjects);

	bool AreSeparatedByActiveObject(const Vector2f& entityPosition1, const Vector2f& entityPosition2) const;

	const std::vector<InteractableObject*>& GetInteractableObjects() const;
	InteractableObject* GetClosestInteractableObject(const Vector2f& playerPos, int playerFloor) const;

	void Reset() const;
private:
	std::vector<EnvironmentActiveObject>	m_EnvironmentActiveObjects{};
	std::vector<EnvironmentCosmeticObject>	m_EnvironmentCosmeticObjects{};
	std::vector<InteractableObject*>		m_pInteractableObjects{};
};
