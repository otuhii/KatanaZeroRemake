#include "pch.h"
#include "Player.h"

Player::Player(Sprite* sprite) 
	: Entity(sprite), m_State{PlayerState::staying}
{
	InitializePlayerSpriteFrames();
	GetSprite()->SetCurrentAnimationState(m_PlayerSpriteFrames[static_cast<int>(m_State)]);
}

void Player::Draw(const Vector2f& drawPos) const
{
	GetSprite()->Draw(drawPos);
}

void Player::Update(float elapsedSec)
{
	GetSprite()->SetCurrentAnimationState(m_PlayerSpriteFrames[static_cast<int>(m_State)]);
}

void Player::SetState(PlayerState state)
{
	m_State = state;
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
