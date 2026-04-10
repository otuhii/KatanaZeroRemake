#pragma once
#include "Entity.h"

#include <vector>

class Enemy final : public Entity
{
public:
	enum class EnemyType {
		grunt,
		gangster,

		count
	};

	enum class EnemyState {
		idle,
		walk,
		run,
		attack,
		turn
	};

	Enemy(EnemyType type, Sprite* sprite, const std::vector<AnimationFrameInfo>& playerAnimation, const Vector2f& position, float speed, float scale);

	void Draw() const override;

	void Update(float elapsedSec, const Vector2f& playerPos, const Rectf& viewport);

	void UpdatePath(const std::vector<Vector2f>& controlPoint);
private:
	EnemyState m_State;
	EnemyType  m_Type;

	std::vector<AnimationFrameInfo> m_EnemySpriteFrames{};
	std::vector<Vector2f>			m_Path{};

	int 
		m_CurrentTargetControlPoint{ 0 };

	void UpdateSprite();

	void SetState(EnemyState state);

	void HandleCurrentState(float elapsedSec, const Vector2f& playerPos);
	void PlayerSensing(const Vector2f& playerPos);

	void Patrol(float elapsedSec);
	void Chase(float elapsedSec, const Vector2f& playerPos);
};

