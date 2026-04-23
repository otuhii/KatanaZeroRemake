#include "pch.h"

#include "CombatManager.h"
#include "Player.h"
#include "EnemyManager.h"
#include "ParticleManager.h"
#include "AttackParticle.h"

#include <iostream>

void CombatManager::ResolveCombat(
	Player* pPlayer,
	EnemyManager* enemyManager,
	ParticleManager* particleManager
)
{	
	for (AttackParticle* pAttackParticle : particleManager->GetParticles())
	{
		if (pAttackParticle->IsActive())
		{
			if (pAttackParticle->GetOwnerType() == AttackParticle::OwnerType::Player)
			{
				for (Enemy* pEnemy : enemyManager->GetEnemies())
				{
					if (pEnemy->IsAlive())
					{
						if (UserUtils::IsPolyInRectRaycast(pAttackParticle->GetWorldCoordinates(), pEnemy->GetHitbox()))
						{
							pEnemy->Kill();
							pAttackParticle->Deactivate();
						}
					}
				}
			}
			else if (pAttackParticle->GetOwnerType() == AttackParticle::OwnerType::Enemy)
			{
				if (UserUtils::IsPolyInRectRaycast(pAttackParticle->GetWorldCoordinates(), pPlayer->GetHitbox()))
				{
					pAttackParticle->Deactivate();
				}
			}

		}
	}
}


