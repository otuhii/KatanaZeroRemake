#include "pch.h"
#include "CollisionManager.h"

#include "utils.h"

void CollisionManager::HandleMovement(Entity* pEntity, const Map& map, float elapsedSec) const
{
	Vector2f
		entityVelocity{ pEntity->GetVelocity() },
		entityPosition{ pEntity->GetPosition() };

	entityPosition.x += entityVelocity.x * elapsedSec;
	pEntity->SetPositionX(entityPosition.x);
	CheckCollision(pEntity, map, true);

	entityPosition.y += entityVelocity.y * elapsedSec;
	pEntity->SetPositionY(entityPosition.y);

	pEntity->SetIsOnGroundState(false); //assuming that player is in the air
	CheckCollision(pEntity, map, false);
}

void CollisionManager::CheckCollision(Entity* pEntity, const Map& map, bool isHorizontalMovement) const
{
	const float
		objectTriggerRange{ 200.f };

	for (const EnvironmentObject& object : map.GetEnvironmentObjects())
	{
		for (const Rectf& collider : object.GetColliders())
		{
			HandleAABB(pEntity, collider, isHorizontalMovement);
		}
	}
}

void CollisionManager::HandleAABB(Entity* pEntity, const Rectf& objectCollider, bool isHorizontalMovement) const
{
	Rectf
		entityHitbox{ pEntity->GetHitbox() };

	const float
		velEps{ 0.1f },
		maxStepHeight{ 15.f };

	if (isHorizontalMovement) //shrinking hitbox in the bottom part because otherwise collision 
							  //is triggered when he is simply standing on the floor
	{
		float shrinkValue{ 2.f };

		entityHitbox.bottom += shrinkValue;
		entityHitbox.height -= shrinkValue;
	}


	if (utils::IsOverlapping(entityHitbox, objectCollider))
	{
		if (isHorizontalMovement)
		{
			float 
				overlapHeight{ (objectCollider.bottom + objectCollider.height) - entityHitbox.bottom };

			if (overlapHeight <= maxStepHeight && overlapHeight > 0) //step-up logic which checks if current
				//overlapped wall can be handled as a stair step
			{
				pEntity->SetPositionY(objectCollider.bottom + objectCollider.height);
				pEntity->UpdateHitbox();
				return;
			}



			if (pEntity->GetVelocityX() > velEps)
			{
				pEntity->SetPositionX(objectCollider.left - entityHitbox.width);
			}
			else if (pEntity->GetVelocityX() < -velEps)
			{
				pEntity->SetPositionX(objectCollider.left + objectCollider.width);
			}
			pEntity->SetVelocityX(0.f);
		}
		else
		{
			if (pEntity->GetVelocityY() > 0)
			{
				pEntity->SetPositionY(objectCollider.bottom - entityHitbox.height);
			}
			else 
			{
				pEntity->SetPositionY(objectCollider.bottom + objectCollider.height);
				pEntity->SetIsOnGroundState(true);
			}
			pEntity->SetVelocityY(0.f);
		}

	}
	pEntity->UpdateHitbox();
}
