#include "pch.h"
#include "CollisionManager.h"
#include "ParticleManager.h"


#include "utils.h"
#include "UserUtils.h"

void CollisionManager::HandleMovement(Entity* pEntity, const Map* pMap, float elapsedSec, bool updateFloorInfo) const
{
	Vector2f
		entityVelocity{ pEntity->GetVelocity() },
		entityPosition{ pEntity->GetPosition() };

	entityPosition.x += entityVelocity.x * elapsedSec;
	pEntity->SetPositionX(entityPosition.x);
	CheckCollision(elapsedSec, pEntity, pMap, true, updateFloorInfo);

	entityPosition.y += entityVelocity.y * elapsedSec;
	pEntity->SetPositionY(entityPosition.y);
	pEntity->SetIsOnGroundState(false); //assuming that player is in the air
	CheckCollision(elapsedSec, pEntity, pMap, false, updateFloorInfo);
}

void CollisionManager::HandleParticles(ParticleManager* pParticleManager, const Map* pMap)
{
	for (AttackParticle* pParticle : pParticleManager->GetParticles())
	{
		if (pParticle->IsActive())
		{
			if (pParticle->GetAttackType() == AttackParticle::AttackType::bullet)
			{
				if (IsOverlappingWithMap(pParticle->GetWorldCoordinates(), pMap))
				{
					pParticle->Deactivate();
				}
			}
		}
	}
}

void CollisionManager::CheckCollision(float elapsedSec, Entity* pEntity, const Map* pMap, bool isHorizontalMovement, bool updateFloorInfo) const
{
	for (const EnvironmentActiveObject& object : pMap->GetEnvironmentActiveObjects())
	{
		for (const Rectf& collider : object.GetColliders())
		{
			HandleAABB(elapsedSec, pEntity, object.GetType(), collider, object.GetFloor(), isHorizontalMovement, updateFloorInfo);
		}
	}
}

void CollisionManager::HandleAABB(float elapsedSec, Entity* pEntity, EnvironmentActiveObject::EnvironmentObjectType type, const Rectf& objectCollider, int objectFloor, bool isHorizontalMovement, bool updateFloorInfo) const
{
	Rectf
		entityCurrentHitbox{ pEntity->GetCurrentHitbox() },
		entityPreviousHitbox{ pEntity->GetPreviousHitbox() };

	const float
		maxStepHeight{ 7.f };

	if (type == EnvironmentActiveObject::EnvironmentObjectType::jumpThroughPlatform)
	{
		if (CanMoveThroughPlatform(elapsedSec, pEntity, objectCollider, isHorizontalMovement))
		{
			return;
		}
	}

	if (isHorizontalMovement) //shrinking hitbox in the bottom part because otherwise collision 
		//is triggered when he is simply standing on the floor
	{
		float shrinkValue{ 2.f };

		entityCurrentHitbox.bottom += shrinkValue;
		entityCurrentHitbox.height -= shrinkValue;
	}

	if (utils::IsOverlapping(entityCurrentHitbox, objectCollider))
	{
		if (isHorizontalMovement)
		{
			float
				overlapHeight{ (objectCollider.bottom + objectCollider.height) - entityCurrentHitbox.bottom };

			if (overlapHeight <= maxStepHeight && 
				overlapHeight > 0 && 
				pEntity->IsOnGround()) 
				//step-up logic which checks if current
				//overlapped wall can be handled as a stair step
			{
				pEntity->SetPositionY(objectCollider.bottom + objectCollider.height);
				pEntity->SetVelocityY(0.f);
				pEntity->SetIsOnGroundState(true);
				return;
			}



			if (entityPreviousHitbox.left + entityPreviousHitbox.width <= objectCollider.left)
			{
				pEntity->SetPositionX(objectCollider.left - entityCurrentHitbox.width*0.5f);
			}
			else if (entityPreviousHitbox.left >= objectCollider.left + objectCollider.width)
			{
				pEntity->SetPositionX(objectCollider.left + objectCollider.width + entityCurrentHitbox.width*0.5f );
			}
			pEntity->SetVelocityX(0.f);
		}
		else if (!isHorizontalMovement)
		{
			if (entityPreviousHitbox.bottom + entityPreviousHitbox.height <= objectCollider.bottom)
			{
				pEntity->SetPositionY(objectCollider.bottom - entityCurrentHitbox.height);
			}
			else 
			{
				float 
					platformTop{objectCollider.bottom + objectCollider.height},
					heightDifference{ 5.f };

				if ((entityCurrentHitbox.bottom > platformTop - heightDifference) &&
					updateFloorInfo) // check if we actually landed on platform top
				{
					pEntity->SetFloor(objectFloor);
				}

				pEntity->SetPositionY(platformTop);
				pEntity->SetIsOnGroundState(true);
			}
			pEntity->SetVelocityY( 0.f );
		}
	}
	
}

bool CollisionManager::CanMoveThroughPlatform(float elapsedSec, Entity* pEntity, const Rectf& objectCollider, bool isHorizontalMovement) const
{
	const float
		velEps{ 0.1f };

	Vector2f
		entityVelocity{ pEntity->GetVelocity() };
	Rectf
		entityHitbox{ pEntity->GetCurrentHitbox() };

	if (isHorizontalMovement)
	{
		return true;
	}

	if (entityVelocity.y > velEps)
	{
		return true;
	}

	if (pEntity->CanJumpThroughPlatform())
	{
		return true;
	}

	float
		platformTop{ objectCollider.bottom + objectCollider.height },
		playerFeet{ entityHitbox.bottom };

	if (playerFeet < platformTop - 15.f)
	{
		return true;
	}

	return false;
}


bool CollisionManager::IsOverlappingWithMap(const std::vector<Vector2f>& poly, const Map* map) const
{
	for (const EnvironmentActiveObject& object : map->GetEnvironmentActiveObjects())
	{
		if (object.GetType() == EnvironmentActiveObject::EnvironmentObjectType::jumpThroughPlatform)
		{
			continue; // can shoot through jumpThroughPlatform
		}

		for (const Rectf& collider : object.GetColliders())
		{
			if (UserUtils::IsPolyInRectAABB(poly, collider))
			{
				return true;
			}
		}
	}

	return false;
}
