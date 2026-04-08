#include "pch.h"
#include "Player.h"
#include "AnimationFrameInfo.h"

#include "UserUtils.h"
#include "utils.h"

#include "Matrix2x3.h"


Player::Player(Sprite* sprite, Sprite* splashSprite, const std::vector<AnimationFrameInfo>& playerAnimation, const Vector2f& position, float speed)
	: Entity(sprite, position, Vector2f{}, speed),
	m_SplashSprite(splashSprite),
	m_State(PlayerState::staying),
	m_PlayerSpriteFrames(playerAnimation)
{
	GetSprite()->SetAnimationFrameInfo(m_PlayerSpriteFrames[static_cast<int>(m_State)]);

	m_SplashSprite->SetAnimationFrameInfo(m_PlayerSpriteFrames[static_cast<int>(PlayerState::attackSplash)]);
	m_SplashSprite->SetVisible(false);

	GetSprite()->SetScale(2.f);
	m_SplashSprite->SetScale(2.f);

	UpdateHitbox();
}

void Player::Draw() const
{
	GetSprite()->Draw(GetPosition(), true, false);
	DrawSplash();

	utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	utils::DrawRect(GetHitbox());
}

void Player::Update(float elapsedSec, const Uint8* pStates, const Rectf& viewport)
{
	HandleKeyboard(pStates, elapsedSec);
	Entity::Update(elapsedSec, viewport);
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

void Player::ProcessMouseUpEvent(const SDL_MouseButtonEvent & e, const Rectf& viewport)
{
	if (e.button == SDL_BUTTON_LEFT)
	{
		if (m_State != PlayerState::attack)
		{
			SetState(PlayerState::attack);

			if (e.x < GetPositionX()) // TODO i should check it in the middle of the sprite/hitbox
			{
				GetSprite()->FlipHorizontally();
			}
			else
			{
				GetSprite()->ResetHorizontalFlip();
			}

			m_SplashSprite->RotateBy(CalculateSplashRotation(
				Vector2f{ static_cast<float>(e.x), static_cast<float>(e.y) }
			));

			UpdateSplashHitbox();
		}
		
	}
}


std::vector<Vector2f> Player::GetSplashHitbox() const
{
	if (m_State == PlayerState::attack)
	{
		return m_SplashHitboxPoints;
	}
	else
	{
		return std::vector<Vector2f>{};
	}
}


// TODO write down my state system so it will be easier to program
void Player::ProcessStateChange(bool isMoving, bool roll)
{
	const float
		velocityThreshold{ 0.1f };

	if (m_State == PlayerState::attack || m_State == PlayerState::roll)
	{
		if (GetSprite()->IsFinished())
		{
			if (m_State == PlayerState::roll)
			{
				SetVelocityX(0.f);
			}

			SetState(PlayerState::staying);
			m_SplashSprite->SetVisible(false);
			m_SplashSprite->ResetAnimation();
		}
		else if (m_State == PlayerState::attack)
		{
			int
				renderSplashAfterFrame{ 2 };

			if (GetSprite()->GetCurrentFrameCount() >= renderSplashAfterFrame)
			{
				m_SplashSprite->SetVisible(true);
			}
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

		return;
	}
	
	if (roll)
	{
		SetState(PlayerState::roll);
		return;
	}

	switch (m_State)
	{
	case PlayerState::jump:
	case PlayerState::fall:
		SetState(PlayerState::staying);
		break;
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
			SetState(PlayerState::afterRun);
		}
		break;
	}
	case PlayerState::run:
	{
		if (!isMoving)
		{
			SetState(PlayerState::afterRun);
		}
		break;
	}
	case PlayerState::afterRun:
	{
		if (isMoving)
		{
			SetState(PlayerState::beforeRun);
		}
		else if (GetSprite()->IsFinished())
		{
			SetState(PlayerState::staying);
		}
	}
	}
}

void Player::HandleKeyboard(const Uint8* pStates, float elapsedSec)
{
	if (m_State != PlayerState::roll)
	{
		if (pStates[SDL_SCANCODE_D])
		{
			if (pStates[SDL_SCANCODE_S])
			{
				float rollSpeed{ 500.f }; //TODO change this to more robust option because now my roll is really dependant on animation which is apparently not so good

				SetVelocityX(rollSpeed);
				GetSprite()->ResetHorizontalFlip();
			}
			else
			{
				SetVelocityX(GetSpeed());
				GetSprite()->ResetHorizontalFlip();
			}
		}
		else if (pStates[SDL_SCANCODE_A])
		{
			if (pStates[SDL_SCANCODE_S])
			{
				float rollSpeed{ 500.f }; //TODO change this to more robust option because now my roll is really dependant on animation which is apparently not so good

				SetVelocityX(-rollSpeed);
				GetSprite()->FlipHorizontally();
			}
			else
			{
				SetVelocityX(-GetSpeed());
				GetSprite()->FlipHorizontally();
			}
		}
		else
		{
			float
				friction{ 1200.f },
				currentVelocityX{ GetVelocityX() },
				velEps{ 0.1f };

			if (std::abs(currentVelocityX) > velEps)
			{
				float
					newVelocityX{ currentVelocityX - (currentVelocityX / std::abs(currentVelocityX) * friction * elapsedSec) };

				if ((currentVelocityX > 0 && newVelocityX < 0) ||
					(currentVelocityX < 0 && newVelocityX > 0))
				{
					newVelocityX = 0.f;
				}
				SetVelocityX(newVelocityX);
			}
		}

		if (pStates[SDL_SCANCODE_W])
		{
			if (IsOnGround())
			{
				SetVelocityY(450.f);
				SetIsOnGroundState(false); //TODO CHANGE HANDLING
			}
		}
	}

	ProcessStateChange(
		(pStates[SDL_SCANCODE_D] || pStates[SDL_SCANCODE_A]),
		(pStates[SDL_SCANCODE_D] && pStates[SDL_SCANCODE_S]) || (pStates[SDL_SCANCODE_A] && pStates[SDL_SCANCODE_S])
	);
}

void Player::DrawSplash() const
{
	Vector2f
		splashOrigin{ GetPosition() };

	if (m_SplashSprite->IsVisible())
	{
		splashOrigin.y += (GetSprite()->GetCurrentFrameDimensions().height) * 0.75f;

		const float
			offset{ 30.f };

		//offseting origin depending on the angle of splash rotation
		float rotAngle{ static_cast<float>(M_PI / 180.f) * m_SplashSprite->GetRotation() };

		splashOrigin.x += std::cosf(rotAngle) * offset;
		splashOrigin.y += std::sinf(rotAngle) * offset;


		if (GetSprite()->IsFlippedHorizontally())
		{
			m_SplashSprite->FlipVertically();
		}
		else
		{
			m_SplashSprite->ResetVerticalFlip();
		}
	}

	m_SplashSprite->Draw(splashOrigin, true, true);

	if (m_SplashSprite->IsVisible())
	{
		utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
		utils::DrawPolygon(m_SplashHitboxPoints);
	}
}

float Player::CalculateSplashRotation(const Vector2f& mouseVec)
{
	Vector2f 
		splashOrigin{ GetPosition() };

	splashOrigin.y += (GetSprite()->GetCurrentFrameDimensions().height) * 0.5f;

	Vector2f
		direction{ mouseVec - splashOrigin };

	float
		angle{ static_cast<float>(180.f / M_PI) * std::atan2(direction.y, direction.x)};

	return angle;
}


//SIMPLE IMPLEMENTATION OF ATTACK BOX AT THE TIP OF THE ATTACK SPRITE
//	i am not sure if it is a good thing to generate hitboxes 
// and like physics related stuff based on animation of the attack 

//Rectf Player::GetAttackHitbox() const
//{
//	const float
//		attackRange{ 70.f },
//		splashRotationAngle{ (float(M_PI) / 180.f) * m_SplashSprite->GetRotation() };
//
//	Vector2f 
//		anchorPoint{ GetPosition() };
//
//	anchorPoint.y += (GetSprite()->GetCurrentFrameDimensions().height) * 0.75f;
//
//	Vector2f hitboxCenter{
//		anchorPoint.x + std::cosf(splashRotationAngle) * attackRange,
//		anchorPoint.y + std::sinf(splashRotationAngle) * attackRange
//	};
//
//	float size{ 80.f };
//
//	return Rectf{
//		hitboxCenter.x - size * 0.5f,
//		hitboxCenter.y - size * 0.5f,
//		size,
//		size
//	};
//
//
//}

void Player::UpdateSplashHitbox()
{
	const std::vector<Vector2f> baseSplashHitbox{
		Vector2f{0.f, 10.f},
		Vector2f{0.f, -10.f},
		Vector2f{100.f, -20.f},
		Vector2f{100.f, 20.f}
	};

	const float
		rotationAngle{ m_SplashSprite->GetRotation() },
		offset{ 30.f };

	Vector2f
		anchorPoint{ GetPosition() };
		
	anchorPoint.y += (GetSprite()->GetCurrentFrameDimensions().height) * 0.75f;

	anchorPoint.x += std::cosf(static_cast<float>(M_PI / 180.f) * rotationAngle) * offset;
	anchorPoint.y += std::sinf(static_cast<float>(M_PI / 180.f) * rotationAngle) * offset;

	Matrix2x3
		hitboxRotation{},
		hitboxTranslation{},
		hitboxVerticalFlip{},
		transformation{};

	hitboxRotation.SetAsRotate(rotationAngle);
	hitboxVerticalFlip.SetAsScale(1, -1);
	hitboxTranslation.SetAsTranslate(anchorPoint);

	if (m_SplashSprite->IsFlippedVertically())
	{
		transformation = hitboxTranslation * hitboxRotation * hitboxVerticalFlip;
	}
	else
	{
		transformation = hitboxTranslation * hitboxRotation;
	}

	m_SplashHitboxPoints = transformation.Transform(baseSplashHitbox);
}

