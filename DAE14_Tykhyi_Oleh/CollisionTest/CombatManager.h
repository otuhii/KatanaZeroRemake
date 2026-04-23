#pragma once
#include "UserUtils.h"

class ParticleManager;
class EnemyManager;
class Map;
class Player;


class CombatManager final
{
public:
	void ResolveCombat(Player* pPlayer, EnemyManager* enemyManager, ParticleManager* particleManager, Map* map);
};

