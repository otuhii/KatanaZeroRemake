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
		attackSplash,

		count
	};

	Player(Sprite* sprite, Sprite* splashSprite, const std::vector<AnimationFrameInfo>& playerAnimation, const Vector2f& position, float speed);

	void Draw() const override;

	void Update(float elapsedSec, const Uint8* pStates, const Rectf& viewport);

	void SetState(PlayerState state);

	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);
private:
	PlayerState m_State;
	
	std::vector<AnimationFrameInfo> m_PlayerSpriteFrames{};

	Sprite* m_SplashSprite;

	void ProcessStateChange(bool isMoving);
	void HandleKeyboard(const Uint8* pStates);
};

