#pragma once
#include "Entity.h"
#include "Map.h"

class ParticleManager;
class SoundManager;

class CollisionManager final
{
public:
	void HandleMovement(Entity* pEntity, const Map* pMap, float elapsedSec, bool updateFloorInfo) const;

	void HandleParticles(ParticleManager* particleManager, SoundManager* pSoundManager, const Map* pMap);
private:
	void CheckCollision(float elapsedSec, Entity* pEntity, const Map* pMap, bool isHorizontalMovement, bool updateFloorInfo) const;
	void HandleAABB(float elapsedSec, Entity* pEntity, EnvironmentActiveObject::EnvironmentObjectType type, const Rectf& objectCollider, int objectFloor, bool isHorizontalMovement, bool updateFloorInfo) const;

	bool CanMoveThroughPlatform(float elapsedSec, Entity* pEntity, const Rectf& objectCollider, bool isHorizontalMovement) const;

	bool IsOverlappingWithMap(const std::vector<Vector2f>& poly, const Map* pMap) const;
};

