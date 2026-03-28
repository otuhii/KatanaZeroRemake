#include "pch.h"
#include "Player.h"

Player::Player(Sprite* sprite, const Vector2f& position, float speed) 
	: Entity(sprite, position, Vector2f{}, speed), m_State(PlayerState::staying)
{
	InitializePlayerSpriteFrames();
	GetSprite()->SetCurrentAnimationState(m_PlayerSpriteFrames[static_cast<int>(m_State)]);
}

void Player::Draw() const
{
	GetSprite()->Draw(GetPosition());
}

void Player::Update(float elapsedSec, const Uint8* pStates)
{
	Entity::Update(elapsedSec);
	HandleKeyboard(pStates);
}

void Player::SetState(PlayerState state)
{
	if (m_State == state)
	{
		return; // no animation flickering
	}

	m_State = state;
	GetSprite()->SetCurrentAnimationState(m_PlayerSpriteFrames[static_cast<int>(m_State)]);
}


void Player::ProcessStateChange(bool isMoving)
{
	switch (m_State)
	{
	case PlayerState::staying:
	{
		if (isMoving)
		{
			SetState(PlayerState::beforeRun);
		}
		break;
	}
	case PlayerState::beforeRun:
	{
		if (GetSprite()->IsFinished())
		{
			SetState(PlayerState::run);
		}

		if (!isMoving)
		{
			SetState(PlayerState::staying);
		}
		break;
	}
	case PlayerState::run:
	{
		if (!isMoving)
		{
			SetState(PlayerState::staying);
		}
	}
	}
}

void Player::HandleKeyboard(const Uint8* pStates)
{
	if (pStates[SDL_SCANCODE_D])
	{
		SetVelocityX(1);
		GetSprite()->ResetHorizontalFlip();
	}
	else if (pStates[SDL_SCANCODE_A])
	{
		SetVelocityX(-1);
		GetSprite()->FlipHorizontally();
	}
	else
	{
		SetVelocityX(0);
	}
	ProcessStateChange((pStates[SDL_SCANCODE_D] || pStates[SDL_SCANCODE_A]));
}

void Player::InitializePlayerSpriteFrames()
{
	const Vector2f
		firstThreeRowsDims{ 42.f, 42.f },
		fourthRowDims{ 52.f, 32.f },
		fifthRowDims{ 48.f, 48.f },
		sixthRowDims{ 52.f, 39.f },
		splashDims{ 100.f, 28.f };


	m_PlayerSpriteFrames.resize(static_cast<size_t>(PlayerState::count));

	m_PlayerSpriteFrames[static_cast<int>(PlayerState::staying)] = Sprite::CurrentAnimationState{
		Rectf{0.f, 0.f, firstThreeRowsDims.x, firstThreeRowsDims.y},
		0.1f,
		10
	};
	m_PlayerSpriteFrames[static_cast<int>(PlayerState::beforeRun)] = Sprite::CurrentAnimationState{
		Rectf{0.f, 42.f, firstThreeRowsDims.x, firstThreeRowsDims.y},
		0.1f,
		4
	};
	m_PlayerSpriteFrames[static_cast<int>(PlayerState::run)] = Sprite::CurrentAnimationState{
		Rectf{0.f, 84.f, firstThreeRowsDims.x, firstThreeRowsDims.y},
		0.1f,
		10
	};
	m_PlayerSpriteFrames[static_cast<int>(PlayerState::roll)] = Sprite::CurrentAnimationState{
		Rectf{0.f, 126.f, fourthRowDims.x, fourthRowDims.y},
		0.1f,
		7
	};
	m_PlayerSpriteFrames[static_cast<int>(PlayerState::jump)] = Sprite::CurrentAnimationState{
		Rectf{0.f, 158.f, fifthRowDims.x, fifthRowDims.y},
		0.1f,
		4
	};
	m_PlayerSpriteFrames[static_cast<int>(PlayerState::fall)] = Sprite::CurrentAnimationState{
		Rectf{192.f, 158.f, fifthRowDims.x, fifthRowDims.y},
		0.1f,
		4
	};
	m_PlayerSpriteFrames[static_cast<int>(PlayerState::attack)] = Sprite::CurrentAnimationState{
		Rectf{0.f, 206.f, sixthRowDims.x, sixthRowDims.y},
		0.1f,
		7
	};
	/*m_PlayerSpriteFrames[static_cast<int>(PlayerState::sp)] = Sprite::CurrentAnimationState{
		Rectf{0.f, 245.f, splashDims.x, splashDims.y},
		1.f,
		5
	};*/ //TODO change splash handling and maybe i should not hardcode x and y values for player sprite frames
}
