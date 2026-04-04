#pragma once
#include "Entity.h"
#include "Map.h"

class CollisionManager final
{
public:
	void HandleMovement(Entity* pEntity, const Map& map, float elapsedSec) const;

private:
	void CheckCollision(Entity* pEntity, const Map& map, bool isHorizontalMovement) const;
	void HandleAABB(Entity* pEntity, const Rectf& objectCollider, bool isHorizontalMovement) const;
};

