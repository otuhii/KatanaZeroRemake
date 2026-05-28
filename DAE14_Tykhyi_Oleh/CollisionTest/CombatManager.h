#pragma once
#include "UserUtils.h"

class ParticleManager;
class SoundManager;
class EnemyManager;
class Map;
class Player;
class Entity;
class AttackParticle;

class CombatManager final
{
public:
	static void ResolveCombat(Player* pPlayer, EnemyManager* enemyManager, ParticleManager* particleManager, Map* pMap, SoundManager* pSoundManager);

	static Vector2f CalculateHitImpulse(const Entity* pHitter, const Entity* pReceiver, const AttackParticle* pParticle);
};

