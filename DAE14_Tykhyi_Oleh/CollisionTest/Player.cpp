#include "pch.h"
#include "Player.h"
#include "AnimationFrameInfo.h"

#include <iostream>

Player::Player(Sprite* sprite, Sprite* splashSprite, const std::vector<AnimationFrameInfo>& playerAnimation, const Vector2f& position, float speed)
	: Entity(sprite, position, Vector2f{}, speed),
	m_SplashSprite(splashSprite),
	m_State(PlayerState::staying),
	m_PlayerSpriteFrames(playerAnimation)
{
	GetSprite()->SetAnimationFrameInfo(m_PlayerSpriteFrames[static_cast<int>(m_State)]);

	m_SplashSprite->SetAnimationFrameInfo(m_PlayerSpriteFrames[static_cast<int>(PlayerState::attackSplash)]);
	m_SplashSprite->SetVisible(false);
}

void Player::Draw() const
{
	GetSprite()->Draw(GetPosition());
	m_SplashSprite->Draw(GetPosition()); //TODO fix position for it
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
	GetSprite()->ResetAnimation();
	m_SplashSprite->ResetAnimation();
}

void Player::ProcessMouseUpEvent(const SDL_MouseButtonEvent & e)
{
	if (e.button == SDL_BUTTON_LEFT)
	{
		SetState(PlayerState::attack);
	}
}

// TODO write down my state system so it will be easier to program
void Player::ProcessStateChange(bool isMoving)
{
	const float
		velocityThreshold{ 0.1f };
	if (m_State == PlayerState::attack)
	{
		int
			renderSplashAfterFrame{ 2 };

		if (GetSprite()->IsFinished())
		{
			SetState(PlayerState::staying);
			m_SplashSprite->SetVisible(false);
			m_SplashSprite->ResetAnimation();
		}
		else if (GetSprite()->GetCurrentFrameCount() >= renderSplashAfterFrame)
		{
			m_SplashSprite->SetVisible(true);
		}

		return;
	}

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
