#pragma once
#include "pch.h"
#include "Player.h"
#include "Enemy.h"

#include "CosmeticParticle.h"
#include "SoundManager.h"

enum class ReplayParticleType {
	bullet,
	melee,
	thrownObject,
	cosmetic
};

struct PlayerSnapshot {
	Vector2f position{};
	Player::PlayerState state{};
	int currentFrame{};
	bool isFlipped{};

	bool isDrawingSplash{};
	float splashRotation{};
	int currentSplashFrame{};
};

struct EnemySnapshot {
	Vector2f position{};
	Enemy::EnemyState state{};
	int currentFrame{};
	bool isFlipped{};
};

struct ReplayParticleEvent {
	ReplayParticleType type{};
	Vector2f position{};
	Vector2f positionOffset{};
	Vector2f velocity{};
	float rotationAngle{};
	float lifetime{};
	bool isFlippedHorizontally{};
	bool isFlippedVertically{};
	Sprite* pSprite{};

	CosmeticParticle::CosmeticParticleType cosmeticType{};
	bool applyGravity{};
};

struct ReplayFrame {
	PlayerSnapshot player{};

	std::vector<EnemySnapshot> enemies{};
	std::vector<SoundManager::SoundEffectType> sounds{};
	std::vector<ReplayParticleEvent*> particleEvents{};
};