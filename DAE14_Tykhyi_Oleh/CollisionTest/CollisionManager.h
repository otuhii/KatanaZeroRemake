#pragma once
#include "Entity.h"
#include "Map.h"

class CollisionManager final
{
public:
	void HandleMovement(Entity* pEntity, const Map& map, float elapsedSec, bool updateFloorInfo) const;

private:
	void CheckCollision(Entity* pEntity, const Map& map, bool isHorizontalMovement, bool updateFloorInfo) const;
	void HandleAABB(Entity* pEntity, EnvironmentActiveObject::EnvironmentObjectType type, const Rectf& objectCollider, int objectFloor, bool isHorizontalMovement, bool updateFloorInfo) const;

	bool CanMoveThroughPlatform(Entity* pEntity, const Rectf& objectCollider, bool isHorizontalMovement) const;
};

