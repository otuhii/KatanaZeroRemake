#include "pch.h"

#include "CombatManager.h"
#include "Player.h"
#include "EnemyManager.h"
#include "ParticleManager.h"
#include "AttackParticle.h"
#include "Map.h"


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
								pEnemy->Kill();

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
					pPlayer->Kill();
					pAttackParticle->Deactivate();
				}
			}

		}
	}
}


