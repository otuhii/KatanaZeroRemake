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

void Player::Update(float elapsedSec, const Uint8* pStates, const Rectf& viewport)
{
	HandleKeyboard(pStates);

	Entity::Update(elapsedSec, viewport);

	if (GetPosition().y < 0) // TODO proper collision check with every obstacle and not just 0 coord
	{
		SetPositionY(0.f);
		SetVelocityY(0.f);
		SetIsOnGroundState(true);
	}
	else
	{
		SetIsOnGroundState(false);
	}
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

//void Player::DebugState()
//{
//	system("CLS");
//
//	std::cout << GetVelocity() << std::endl;
//
//	switch (m_State)
//	{
//	case PlayerState::staying:
//		std::cout << "STAYING STATE" << std::endl;
//		break;
//	case PlayerState::beforeRun:
//		std::cout << "BEFORE RUN STATE" << std::endl;
//		break;
//	case PlayerState::run:
//		std::cout << "RUN STATE" << std::endl;
//		break;
//	case PlayerState::roll:
//		std::cout << "ROLL STATE" << std::endl;
//		break;
//	case PlayerState::jump:
//		std::cout << "JUMP STATE" << std::endl;
//		break;
//	case PlayerState::fall:
//		std::cout << "FALL STATE" << std::endl;
//		break;
//	case PlayerState::attack:
//		std::cout << "ATTACK STATE" << std::endl;
//		break;
//	}
//}

// TODO write down my state system so it will be easier to program
void Player::ProcessStateChange(bool isMoving)
{
	const float
		velocityThreshold{ 0.1f };
	//TODO consider using std::numeric_limits<float>::epsilon() and not this 0.1f value

	if (!IsOnGround())
	{
		if (GetVelocityY() > velocityThreshold)
		{
			SetState(PlayerState::jump);
		}
		else
		{
			SetState(PlayerState::fall);
		}
	}
	else
	{
		if (m_State == PlayerState::jump || m_State == PlayerState::fall)
		{
			SetState(PlayerState::staying);
		}

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

	//DebugState();
}

void Player::HandleKeyboard(const Uint8* pStates)
{

	if (pStates[SDL_SCANCODE_D])
	{
		SetVelocityX(GetSpeed());
		GetSprite()->ResetHorizontalFlip();
	}
	else if (pStates[SDL_SCANCODE_A])
	{
		SetVelocityX(-GetSpeed());
		GetSprite()->FlipHorizontally();
	}
	else
	{
		SetVelocityX(0.f);
	}

	if (pStates[SDL_SCANCODE_W])
	{
		if (IsOnGround())
		{
			SetVelocityY(500.f);
			SetIsOnGroundState(false); //TODO CHANGE HANDLING
		}
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
