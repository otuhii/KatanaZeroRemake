#include "pch.h"
#include "Player.h"
#include "AnimationFrameInfo.h"

Player::Player(Sprite* sprite, Sprite* splashSprite, const std::vector<AnimationFrameInfo>& playerAnimation, const Vector2f& position, float speed)
	: Entity(sprite, position, Vector2f{}, speed),
	m_SplashSprite(splashSprite),
	m_State(PlayerState::staying),
	m_PlayerSpriteFrames(playerAnimation)
{
	GetSprite()->SetAnimationFrameInfo(m_PlayerSpriteFrames[static_cast<int>(m_State)]);
	m_SplashSprite->SetAnimationFrameInfo(m_PlayerSpriteFrames[static_cast<int>(PlayerState::attackSplash)]);
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
	GetSprite()->SetAnimationFrameInfo(m_PlayerSpriteFrames[static_cast<int>(m_State)]);
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
