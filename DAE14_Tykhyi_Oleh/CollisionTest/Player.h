#pragma once
#include "Entity.h"
#include <vector>

class Player final : public Entity
{
public:
	enum class PlayerState {
		none=-1, 

		staying,
		beforeRun,
		run,
		roll,
		jump,
		fall,
		attack,

		count
	};

	Player(Sprite* sprite);

	void Draw(const Vector2f& drawPos) const override;

	void Update(float elapsedSec) override;

	void SetState(PlayerState state);

private:
	PlayerState m_State;

	std::vector<Sprite::CurrentAnimationState> m_PlayerSpriteFrames{};

	void InitializePlayerSpriteFrames();
};

