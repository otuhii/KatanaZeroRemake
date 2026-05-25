#pragma once
#include "pch.h"
#include "Player.h"
#include "Enemy.h"


struct PlayerSnapshot {
	Vector2f position;
	Player::PlayerState state;
	int currentFrame;
	bool isFlipped;

	bool isDrawingSplash;
	float splashRotation;
	int currentSplashFrame;
};

struct EnemySnapshot {
	Vector2f position;
	Enemy::EnemyState state;
	int currentFrame;
	bool isFlipped;
};

struct ReplayFrame {
	PlayerSnapshot player;
	std::vector<EnemySnapshot> enemies;
};