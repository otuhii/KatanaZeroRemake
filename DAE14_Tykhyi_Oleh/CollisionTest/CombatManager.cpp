#include "pch.h"

#include "CombatManager.h"
#include "Player.h"
#include "EnemyManager.h"
#include "ParticleManager.h"
#include "Map.h"

#include "InteractableObject.h"
#include "Door.h"

void CombatManager::ResolveCombat(
	Player* pPlayer,
	EnemyManager* enemyManager,
	ParticleManager* particleManager,
	Map* pMap
)
{	
	Vector2f
		playerPos{ pPlayer->GetPosition() };
	playerPos.y += pPlayer->GetCurrentHitbox().height * 0.5f;


	for (AttackParticle* pAttackParticle : particleManager->GetAttackParticles())
	{
		if (pAttackParticle->IsActive())
		{
			if (pAttackParticle->GetOwnerType() == AttackParticle::OwnerType::Player)
			{
				//deflection
				if (pAttackParticle->GetAttackType() == AttackParticle::AttackType::melee)
				{
					for (AttackParticle* pOtherParticle : particleManager->GetAttackParticles())
					{
						if (pOtherParticle->IsActive() &&
							pOtherParticle->GetAttackType() == AttackParticle::AttackType::bullet &&
							pOtherParticle->GetOwnerType() == AttackParticle::OwnerType::Enemy)
						{
							if (UserUtils::ArePolygonsOverlapping(
								pAttackParticle->GetWorldCoordinates(),
								pOtherParticle->GetWorldCoordinates())
								)
							{
								pOtherParticle->Deflect();
							}
						}
					}
				}

				//regular slash
				for (Enemy* pEnemy : enemyManager->GetEnemies())
				{
					Vector2f
						enemyPosition{ pEnemy->GetPosition() };
					enemyPosition.y += pEnemy->GetCurrentHitbox().height * 0.5f;

					if (pEnemy->IsAlive())
					{
						if (!pMap->AreSeparatedByActiveObject(playerPos, enemyPosition))
						{
							if (UserUtils::IsPolyInRectAABB(pAttackParticle->GetWorldCoordinates(), pEnemy->GetCurrentHitbox()))
							{
								pEnemy->Kill(CalculateHitImpulse(pPlayer, pEnemy, pAttackParticle));

								if (pAttackParticle->GetAttackType() == AttackParticle::AttackType::bullet ||
									pAttackParticle->GetAttackType() == AttackParticle::AttackType::thrownObject)
								{
									pAttackParticle->Deactivate(); // deactivating only bullet particle for player because i want to allow multiple kills per one slash
								}
							}
						}
					}
				}


				//door opening and other interactions maybe
				for (InteractableObject* pObject : pMap->GetInteractableObjects())
				{
					if (pObject->GetType() == InteractableObject::InteractableType::door)
					{
						Door* pDoor{ static_cast<Door*>(pObject) };

						if (!pDoor->IsOpened() || !pDoor->IsOpening())
						{
							if (UserUtils::IsPolyInRectRaycast(pAttackParticle->GetWorldCoordinates(), pDoor->GetCurrentCollider()))
							{
								pDoor->Interact();
							}
						}
					}
				}
			}
			else if (pAttackParticle->GetOwnerType() == AttackParticle::OwnerType::Enemy)
			{
				if (UserUtils::IsPolyInRectAABB(pAttackParticle->GetWorldCoordinates(), pPlayer->GetCurrentHitbox()))
				{
					pPlayer->Kill(CalculateHitImpulse(pAttackParticle->GetOwnerEntity(), pPlayer, pAttackParticle));
					pAttackParticle->Deactivate();
				}
			}

		}
	}
}

Vector2f CombatManager::CalculateHitImpulse(const Entity* pHitter,  const Entity* pReceiver, const AttackParticle* pParticle)
{
	Vector2f
		impulse{};

	float
		pushMagnitude{ 800.f };

	if (pParticle->GetAttackType() == AttackParticle::AttackType::bullet)
	{
		impulse = pParticle->GetVelocity().Normalized() * pushMagnitude;
		return impulse;
	}
	else if (pParticle->GetAttackType() == AttackParticle::AttackType::melee)
	{
		impulse = (pReceiver->GetPosition() - pHitter->GetPosition()).Normalized() * pushMagnitude;
		impulse.y *= 0.6f;
		return impulse;
	}

	return Vector2f{ 0.f, 0.f };
}


