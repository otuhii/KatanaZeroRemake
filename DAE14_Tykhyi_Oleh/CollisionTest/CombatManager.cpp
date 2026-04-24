#include "pch.h"

#include "CombatManager.h"
#include "Player.h"
#include "EnemyManager.h"
#include "ParticleManager.h"
#include "Map.h"

#include <iostream>

void CombatManager::ResolveCombat(
	Player* pPlayer,
	EnemyManager* enemyManager,
	ParticleManager* particleManager,
	Map* map
)
{	
	Vector2f
		playerPos{ pPlayer->GetPosition() };
	playerPos.y += pPlayer->GetHitbox().height * 0.5f;


	for (AttackParticle* pAttackParticle : particleManager->GetParticles())
	{
		if (pAttackParticle->IsActive())
		{
			if (pAttackParticle->GetOwnerType() == AttackParticle::OwnerType::Player)
			{
				//deflection
				if (pAttackParticle->GetAttackType() == AttackParticle::AttackType::melee)
				{
					for (AttackParticle* pOtherParticle : particleManager->GetParticles())
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
								std::cout << "Deflect" << std::endl;
							}
						}
					}
				}



				//regulat slash
				for (Enemy* pEnemy : enemyManager->GetEnemies())
				{
					Vector2f
						enemyPosition{ pEnemy->GetPosition() };
					enemyPosition.y += pEnemy->GetHitbox().height * 0.5f;

					if (pEnemy->IsAlive())
					{
						if (!map->AreSeparatedByActiveObject(playerPos, enemyPosition))
						{
							if (UserUtils::IsPolyInRectRaycast(pAttackParticle->GetWorldCoordinates(), pEnemy->GetHitbox()))
							{
								pEnemy->Kill(CalculateHitImpulse(pPlayer, pEnemy, pAttackParticle));

								if (pAttackParticle->GetAttackType() == AttackParticle::AttackType::bullet)
								{
									pAttackParticle->Deactivate(); // deactivating only bullet particle for player because i want to allow multiple kills per one slash
								}
							}
						}
					}
				}
			}
			else if (pAttackParticle->GetOwnerType() == AttackParticle::OwnerType::Enemy)
			{
				if (UserUtils::IsPolyInRectAABB(pAttackParticle->GetWorldCoordinates(), pPlayer->GetHitbox()))
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


