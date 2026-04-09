#include "pch.h"

#include "Player.h"
#include "AnimationFrameInfo.h"
#include "UserUtils.h"

#include "Matrix2x3.h"     
#include "utils.h"


Player::Player(Sprite* sprite, Sprite* splashSprite, const std::vector<AnimationFrameInfo>& playerAnimation, const Vector2f& position, float speed)
	: Entity(sprite, position, Vector2f{}, speed),
	m_SplashSprite(splashSprite),
	m_State(PlayerState::staying),
	m_PlayerSpriteFrames(playerAnimation)
{
	GetSprite()->SetAnimationFrameInfo(m_PlayerSpriteFrames[static_cast<int>(m_State)]);

	m_SplashSprite->SetAnimationFrameInfo(m_PlayerSpriteFrames[static_cast<int>(PlayerState::attackSplash)]);
	m_SplashSprite->SetVisible(false);

	const float
		scale{ 2.f };

	GetSprite()->SetScale(scale);
	m_SplashSprite->SetScale(scale);

	UpdateHitbox();
}

void Player::Draw() const
{
	GetSprite()->Draw(GetPosition(), true, false);
	DrawSplash();

	DrawHitboxes();
}

void Player::Update(float elapsedSec, const Uint8* pStates, const Rectf& viewport)
{
	HandleKeyboard(pStates, elapsedSec);
	UpdateCurrentState(elapsedSec);
	Entity::Update(elapsedSec, viewport);
}

void Player::SetState(PlayerState state)
{
	if (m_State == state) { return; } // no animation flickering

	if (m_State == PlayerState::attack)
	{
		m_SplashSprite->SetVisible(false);
		m_SplashSprite->ResetAnimation();
	}

	if (m_State == PlayerState::attack || m_State == PlayerState::roll)
	{
		SetCanJumpThroughPlatform(false);
	}

	m_State = state;

	GetSprite()->SetAnimationFrameInfo(m_PlayerSpriteFrames[static_cast<int>(m_State)]);
	GetSprite()->ResetAnimation();
	m_SplashSprite->ResetAnimation();
}

void Player::ProcessMouseUpEvent(const SDL_MouseButtonEvent & e, const Rectf& viewport)
{
	Vector2f
		mousePos{ static_cast<float>(e.x), static_cast<float>(e.y) };

	if (e.button == SDL_BUTTON_LEFT)
	{
		if (m_State != PlayerState::attack)
		{
			Attack(mousePos);
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
}

void Player::UpdateCurrentState(float elapsedSec)
{
	switch (m_State)
	{
	case PlayerState::attack:
	{
		UpdateAttackState(elapsedSec);
		break;
	}
	}
}

Player::PlayerState Player::GetNextState(bool isMoving, bool roll, bool crouch)
{
	if (m_State == PlayerState::attack || m_State == PlayerState::roll)
	{
		if (IsSpriteAnimationFinished())
		{
			if (m_State == PlayerState::roll) { SetVelocityX(0.f); }
			return PlayerState::staying;
		}
		return m_State;
	}

	if (!IsOnGround())
	{
		return GetNextAirState();
	}

	return GetNextGroundState(isMoving, roll, crouch);
}

Player::PlayerState Player::GetNextAirState()
{
	if (GetVelocityY() > m_VelEps)
	{
		return PlayerState::jump;
	}
	else
	{
		return PlayerState::fall;
	}
}

Player::PlayerState Player::GetNextGroundState(bool isMoving, bool roll, bool crouch)
{
	if (roll)
	{
		return PlayerState::roll;
	}

	if (m_State == PlayerState::jump || m_State == PlayerState::fall)
	{
		return PlayerState::staying;
	}

	if (m_State == PlayerState::crouch || m_State == PlayerState::postCrouch)
	{
		if (!crouch && m_State == PlayerState::crouch)
		{
			return PlayerState::postCrouch;
		}
		else if (isMoving)
		{
			return PlayerState::beforeRun;
		}
		else if (m_State == PlayerState::postCrouch && IsSpriteAnimationFinished())
		{
			return PlayerState::staying;
		}
		return m_State;
	}

	if (isMoving)
	{
		if (m_State == PlayerState::staying || m_State == PlayerState::afterRun)
		{
			return PlayerState::beforeRun;
		}
		else if (m_State == PlayerState::beforeRun && IsSpriteAnimationFinished())
		{
			return PlayerState::run;
		}
	}
	else
	{
		if (m_State == PlayerState::run || m_State == PlayerState::beforeRun)
		{
			return PlayerState::afterRun;
		}
		else if (m_State == PlayerState::afterRun && IsSpriteAnimationFinished())
		{
			return PlayerState::staying;
		}
		else if (crouch)
		{
			return PlayerState::crouch;
		}
	}
	return m_State;
}

void Player::ProcessStateChange(bool isMoving, bool roll, bool crouch)
{
	PlayerState
		nextState{ GetNextState(isMoving, roll, crouch) };

	SetState(nextState);
}

void Player::HandleKeyboard(const Uint8* pStates, float elapsedSec)
{
	bool
		moveRight	{ static_cast<bool>(pStates[SDL_SCANCODE_D]) },
		moveLeft	{ static_cast<bool>(pStates[SDL_SCANCODE_A]) },
		isMoving	{ static_cast<bool>(moveRight || moveLeft) },
		downButton	{ static_cast<bool>(pStates[SDL_SCANCODE_S]) },
		jumpButton	{ static_cast<bool>(pStates[SDL_SCANCODE_W]) };

	ProcessJumpThroughPlatform(downButton);

	if (m_State != PlayerState::roll)
	{
		HandleVerticalMovement(downButton, jumpButton, elapsedSec);
		HandleHorizontalMovement(moveLeft, moveRight, downButton, elapsedSec);
	}

	ProcessStateChange(
		isMoving,
		isMoving && downButton,
		downButton && !isMoving
	);
}

void Player::Attack(const Vector2f& mousePos)
{
	SetState(PlayerState::attack);
	AttackDash(mousePos);
	SetCanJumpThroughPlatform(true);

	if (mousePos.x < GetPositionX())
	{
		GetSprite()->FlipHorizontally();
	}
	else
	{
		GetSprite()->ResetHorizontalFlip();
	}

	m_SplashSprite->RotateBy(CalculateSplashRotation(
		mousePos
	));
}

void Player::AttackDash(const Vector2f& mousePos)
{
	Vector2f
		playerPos{ GetPosition() };

	Vector2f
		dashDirection{ (mousePos - playerPos).Normalized() };

	float
		dashForce{ 900.f };

	SetVelocity(dashForce * dashDirection);
}

void Player::ProcessJumpThroughPlatform(bool downButton)
{
	if (downButton)
	{
		SetCanJumpThroughPlatform(true);
	}

	if ((CanJumpThroughPlatform() 
		&& !IsOnGround()) || 
		m_State == PlayerState::attack)
	{
		return;
	}

	if (GetVelocityY() > m_VelEps || (IsOnGround() && !downButton))
	{
		SetCanJumpThroughPlatform(false);
	}
}

void Player::HandleVerticalMovement(bool down, bool jump, float elapsedSec)
{
	bool
		speedUpFallIntent{ !IsOnGround() && down },
		jumpIntent{ jump && IsOnGround() };

	if (speedUpFallIntent)
	{
		SetVelocityY(GetVelocityY() - m_FallSpeedUp * elapsedSec);
	}

	if (jumpIntent)
	{
		SetIsOnGroundState(false);
		SetVelocityY(m_JumpImpulse);
	}
}

void Player::HandleHorizontalMovement(bool left, bool right, bool down, float elapsedSec)
{
	bool
		notMoving{ !left && !right },
		rollIntent{ down && IsOnGround() };

	if (notMoving)
	{
		ApplyFriction(elapsedSec);
		return;
	}

	float direction{1.f};
	if (left)
	{
		direction = -1.f;
	}

	float currentSpeed{ GetSpeed() };
	//roll
	if (rollIntent)
	{
		currentSpeed = m_RollSpeed;
	}

	SetVelocityX(direction * currentSpeed);

	if (left)
	{
		GetSprite()->FlipHorizontally();
	}
	else
	{
		GetSprite()->ResetHorizontalFlip();
	}
}

void Player::ApplyFriction(float elapsedSec)
{
	float
		currentVelocityX{ GetVelocityX() };

	if (std::abs(currentVelocityX) > m_VelEps)
	{
		float
			newVelocityX{ currentVelocityX - (currentVelocityX / std::abs(currentVelocityX) * m_FrictionForce * elapsedSec) };

		if ((currentVelocityX > 0 && newVelocityX < 0) ||
			(currentVelocityX < 0 && newVelocityX > 0))
		{
			newVelocityX = 0.f;
		}
		SetVelocityX(newVelocityX);
	}
}

void Player::ApplyAirResistance(float elapsedSec)
{
	Vector2f
		currentVelocity{ GetVelocity() };

	if (currentVelocity.Length() > 10.f)
	{
		currentVelocity -= currentVelocity.Normalized() * m_AirResistance * elapsedSec;
		SetVelocity(currentVelocity);
	}
	else
	{
		SetVelocity(Vector2f{ 0.f, 0.f });
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
		angle{ static_cast<float>(180.f / M_PI) * std::atan2(direction.y, direction.x) };

	return angle;
}


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

void Player::UpdateAttackState(float elapsedSec)
{
	int
		renderSplashAfterFrame{ 2 };
	if (GetSprite()->GetCurrentFrameCount() >= renderSplashAfterFrame)
	{
		m_SplashSprite->SetVisible(true);
	}
	UpdateSplashHitbox();
	ApplyAirResistance(elapsedSec);
}

void Player::DrawHitboxes() const
{
	utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	utils::DrawRect(GetHitbox());

	if (m_SplashSprite->IsVisible())
	{
		utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
		utils::DrawPolygon(m_SplashHitboxPoints);
	}
}

