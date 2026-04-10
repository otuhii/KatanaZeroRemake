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

	void Update(float elapsedSec, const Rectf& viewport);

	void SetState(EnemyState state);
private:
	EnemyState m_State;
	EnemyType  m_Type;

	std::vector<AnimationFrameInfo> m_EnemySpriteFrames{};
	

};

