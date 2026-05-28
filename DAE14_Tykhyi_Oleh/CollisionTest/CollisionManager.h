#pragma once
#include "Entity.h"
#include "Map.h"

class ParticleManager;
class SoundManager;

class CollisionManager final
{
public:
	static void HandleMovement(
		Entity* pEntity, 
		const Map* pMap, 
		float elapsedSec, 
		bool updateFloorInfo
	);

	static void HandleParticles(
		ParticleManager* particleManager, 
		SoundManager* pSoundManager, 
		const Map* pMap
	);
private:
	static void CheckCollision(
		float elapsedSec, 
		Entity* pEntity, 
		const Map* pMap,
		bool isHorizontalMovement,
		bool updateFloorInfo
	);
	static void HandleAABB(
		float elapsedSec, 
		Entity* pEntity, 
		EnvironmentActiveObject::EnvironmentObjectType type, 
		const Rectf& objectCollider,
		int objectFloor, 
		bool isHorizontalMovement, 
		bool updateFloorInfo
	);

	static bool CanMoveThroughPlatform(
		float elapsedSec,
		Entity* pEntity, 
		const Rectf& objectCollider, 
		bool isHorizontalMovement
	);

	static bool IsOverlappingWithMap(
		const std::vector<Vector2f>& poly, 
		const Map* pMap
	);
};

