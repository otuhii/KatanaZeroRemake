#include "pch.h"

#include "Player.h"
#include "AnimationFrameInfo.h"
#include "UserUtils.h"
#include "ParticleManager.h"
#include "SoundManager.h"

#include "Map.h"

#include "InteractableObject.h"
#include "ThrowableObject.h"
#include "Snapshots.h"

#include "VFX.h"


Player::Player(
	Sprite* sprite, 
	Sprite* splashSprite, 
	Sprite* rangeProjectileSprite,
	const std::vector<AnimationFrameInfo>& playerAnimation, 
	const std::vector<AnimationFrameInfo>& rangeProjectileSpriteFrameInfo,
	const Vector2f& position, 
	float speed, 
	float scale, 
	int floor
)
	: Entity(sprite, EntityType::player, position, Vector2f{}, speed, floor),
	m_SplashSprite(splashSprite),
	m_State(PlayerState::staying),
	m_PlayerSpriteFrames(playerAnimation),
	m_RangeProjectileSprite{rangeProjectileSprite}
{
	GetSprite()->SetAnimationFrameInfo(m_PlayerSpriteFrames[static_cast<int>(m_State)]);

	m_SplashSprite->SetAnimationFrameInfo(m_PlayerSpriteFrames[static_cast<int>(PlayerState::attackSplash)]);
	m_SplashSprite->SetVisible(false);

	GetSprite()->SetScale(scale);
	m_SplashSprite->SetScale(scale);

	m_RangeProjectileSprite->SetAnimationFrameInfo(rangeProjectileSpriteFrameInfo[0]);

	UpdateHitbox();
}

void Player::Draw() const
{
	Entity::Draw();
	DrawSplash();
}

void Player::Update(float elapsedSec, Map* pMap, const Uint8* pStates, const Rectf& viewport, ParticleManager* pParticleManager, SoundManager* pSoundManager)
{
	if (m_IsAutoWalking)
	{
		HandleAutowalk(pSoundManager);
		return;
	}
	HandleKeyboard(pMap, pStates, elapsedSec, pParticleManager, pSoundManager);
	UpdateCurrentState(elapsedSec);
	UpdateCooldowns(elapsedSec);
	Entity::Update(elapsedSec, viewport);
}

void Player::SetState(PlayerState state, ParticleManager* pParticleManager, SoundManager* pSoundManager)
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

	if (m_State == PlayerState::fall && state == PlayerState::staying)
	{
		if (pSoundManager)
		{
			pSoundManager->Play(SoundManager::SoundEffectType::land, 0);
		}
	}

	m_State = state;

	if (m_State == PlayerState::attack)
	{
		if (pSoundManager)
		{
			pSoundManager->Play(SoundManager::SoundEffectType::masterswordSlash, 0);
		}
	}

	if (m_State == PlayerState::beforeRun)
	{
		if (pParticleManager)
		{
			VFX::SpawnDust(10, GetPosition(), GetVelocity(), pParticleManager);
		}

		if (pSoundManager)
		{
			pSoundManager->Play(SoundManager::SoundEffectType::playerPrerun, 0);
		}
	}

	if (m_State == PlayerState::roll)
	{
		if (pSoundManager)
		{
			pSoundManager->Play(SoundManager::SoundEffectType::roll, 0);
		}
	}

	if (m_State == PlayerState::jump)
	{
		if (pSoundManager)
		{
			pSoundManager->Play(SoundManager::SoundEffectType::playerJump, 0);
		}
	}

	GetSprite()->SetAnimationFrameInfo(m_PlayerSpriteFrames[static_cast<int>(m_State)]);
	GetSprite()->ResetAnimation();
	m_SplashSprite->ResetAnimation();
}

Player::PlayerState Player::GetState() const
{
	return m_State;
}

void Player::ProcessMouseUpEvent(const SDL_MouseButtonEvent & e, const Vector2f& offset, ParticleManager* particleManager, SoundManager* pSoundManager, const Rectf& viewport)
{
	if (IsAlive())
	{
		Vector2f
			mousePos{ static_cast<float>(e.x) + offset.x, static_cast<float>(e.y)+offset.y };

		if (e.button == SDL_BUTTON_LEFT)
		{
			if (m_State != PlayerState::attack &&
				m_State != PlayerState::catPet && 
				m_AttackCooldownTimer <= 0.f &&
				!m_IsAutoWalking)
			{
				if (m_pHeldObject)
				{
					SpawnThrownObject(particleManager, mousePos);
					m_pHeldObject = nullptr;
					return;
				}
				m_AttackCooldownTimer = m_AttackCooldown;
				Attack(mousePos, particleManager, pSoundManager);
			}
		}
	}
}

void Player::Kill(const Vector2f& impulse)
{
	Entity::Kill(impulse);

	SetState(PlayerState::hurtFly, nullptr, nullptr);
}

ThrowableObject* Player::GetHeldObject() const
{
	return m_pHeldObject;
}

void Player::Reset()
{
	Entity::Reset();
	SetState(PlayerState::staying, nullptr, nullptr);
	m_pHeldObject = nullptr;
}

bool Player::CanDeflect() const
{
	if (m_State == PlayerState::attack)
	{
		const int
			startDeflectionFrame{ 1 },
			endDeflectionFrame{ GetSprite()->GetCurrentMaxFrameCount() };

		return (GetSprite()->GetCurrentFrameCount() > startDeflectionFrame &&
			GetSprite()->GetCurrentFrameCount() < endDeflectionFrame);
	}

	return false;
}

bool Player::IsInsensible() const
{
	/*if (m_State == PlayerState::roll)
	{
		const int
			startInsensibleFrame{ 1 },
			endDeInsensibleFrame{ GetSprite()->GetCurrentMaxFrameCount() };

		return (GetSprite()->GetCurrentFrameCount() > startInsensibleFrame &&
			GetSprite()->GetCurrentFrameCount() < endDeInsensibleFrame);
	}
	return false;*/

	return (m_State == PlayerState::roll);
}

void Player::ApplySnapshot(const PlayerSnapshot* snapshot)
{
	SetPosition(snapshot->position);
	SetState(snapshot->state, nullptr, nullptr);
	
	GetSprite()->SetCurrentFrame(snapshot->currentFrame);
	if (snapshot->isFlipped)
	{
		GetSprite()->FlipHorizontally();
	}
	else
	{
		GetSprite()->ResetHorizontalFlip();
	}

	m_SplashSprite->SetCurrentFrame(snapshot->currentSplashFrame);
	m_SplashSprite->RotateBy(snapshot->splashRotation);
	m_SplashSprite->SetVisible(snapshot->isDrawingSplash);
}

void Player::SetCanFinish(bool canFinish)
{
	m_CanFinish = canFinish;
}

bool Player::CanFinish() const
{
	return m_CanFinish;
}

bool Player::IsSplashDrawn() const
{
	return m_SplashSprite->IsVisible();
}

int Player::GetSplashAnimationFrame() const
{
	return m_SplashSprite->GetCurrentFrameCount();
}

float Player::GetSplashRotation() const
{
	return m_SplashSprite->GetRotation();
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
	if (m_State == PlayerState::hurtFly)
	{
		if (IsSpriteAnimationFinished())
		{
			return PlayerState::hurtOnGround;
		}
		return m_State;
	}

	if (m_State == PlayerState::hurtOnGround)
	{
		return m_State;
	}

	if (m_State == PlayerState::catPet)
	{
		if (IsSpriteAnimationFinished())
		{
			return PlayerState::staying;
		}

		return m_State;
	}

	if (m_State == PlayerState::attack || m_State == PlayerState::roll)
	{
		if (m_State == PlayerState::roll && !IsOnGround())
		{
			return GetNextAirState();
		}

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

void Player::ProcessStateChange(bool isMoving, bool roll, bool crouch, ParticleManager* pParticleManager, SoundManager* pSoundManager)
{
	PlayerState
		nextState{ GetNextState(isMoving, roll, crouch) };

	SetState(nextState, pParticleManager, pSoundManager);
}

void Player::HandleKeyboard(Map* pMap, const Uint8* pStates, float elapsedSec, ParticleManager* pParticleManager, SoundManager* pSoundManager)
{
	bool
		moveRight	{ static_cast<bool>(pStates[SDL_SCANCODE_D]) },
		moveLeft	{ static_cast<bool>(pStates[SDL_SCANCODE_A]) },
		isMoving	{ static_cast<bool>(moveRight || moveLeft) },
		downButton	{ static_cast<bool>(pStates[SDL_SCANCODE_S]) },
		jumpButton	{ static_cast<bool>(pStates[SDL_SCANCODE_W]) },
		interactionButton{ static_cast<bool>(pStates[SDL_SCANCODE_SPACE]) };

	if (interactionButton && IsOnGround() && IsAlive())
	{
		Interact(pMap, pParticleManager, pSoundManager);
		return;
	}

	ProcessJumpThroughPlatform(downButton);

	bool
		rollIntent{ isMoving && downButton && m_RollCooldownTimer <= 0.f };

	if (IsOnGround())
	{
		m_AirAttackCount = 0;
	}

	if (m_State != PlayerState::roll &&
		m_State != PlayerState::hurtFly && 
		m_State != PlayerState::hurtOnGround &&
		m_State != PlayerState::catPet
		)
	{
		HandleVerticalMovement(downButton, jumpButton, elapsedSec);
		HandleHorizontalMovement(moveLeft, moveRight, downButton, elapsedSec);
	}

	ProcessStateChange(
		isMoving,
		rollIntent,
		downButton && !isMoving,
		pParticleManager,
		pSoundManager
	);

	if (rollIntent)
	{
		m_RollCooldownTimer = m_RollCooldown;
	}
}

void Player::HandleAutowalk(SoundManager* pSoundManager)
{
	const float
		targetOffset{ 50.f };

	float
		targetX{m_pTargetObject->GetPosition().x - targetOffset };

	if ((targetX - GetPositionX()) < 0)
	{
		GetSprite()->FlipHorizontally();
	}
	else
	{
		GetSprite()->ResetHorizontalFlip();
	}

	float
		dx{ targetX - GetPositionX()};

	if (std::abs(dx) < m_ArrivalThreshold)
	{
		SetVelocityX(0.f);
		SetPositionX(targetX);
		m_IsAutoWalking = false;

		SetState(PlayerState::catPet, nullptr, nullptr);//TODO hardcoding this because i probably wont use it much for anything else but i can review it later
		GetSprite()->ResetHorizontalFlip();
		if (m_pTargetObject)
		{
			m_pTargetObject->Interact(pSoundManager);
		}
	}
	else
	{
		int
			moveDirection{ 1 };
		if (dx < 0)
		{
			moveDirection = -1;
		}
		SetVelocityX(moveDirection * GetSpeed());
	}

	
}

void Player::Interact(Map* pMap, ParticleManager* pParticleManager, SoundManager* pSoundManager)
{
	if (m_State == PlayerState::catPet || m_IsAutoWalking)
	{
		return;
	}

	InteractableObject*
		pInteractableObject{ pMap->GetClosestInteractableObject(GetPosition(), GetFloor()) };

	if (pInteractableObject != nullptr)
	{
		InteractableObject::InteractableType
			type{ pInteractableObject->GetType() };

		if (type == InteractableObject::InteractableType::cat)
		{
			m_pTargetObject = pInteractableObject;
			m_IsAutoWalking = true;
			SetState(PlayerState::run, pParticleManager, pSoundManager);
		}
		else if (type == InteractableObject::InteractableType::throwableObject)
		{
			if (m_pHeldObject == nullptr)
			{
				pInteractableObject->Interact(pSoundManager);
				m_pHeldObject = static_cast<ThrowableObject*>(pInteractableObject);

			}
		}
	}
}

void Player::Attack(const Vector2f& mousePos, ParticleManager* particleManager, SoundManager* pSoundManager)
{
	SetState(PlayerState::attack, particleManager, pSoundManager);
	if (IsOnGround())
	{
		AttackDash(mousePos);
	}
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

	SpawnAttackParticle(particleManager);

	if (m_RangeAttackCooldownTimer <= 0.f)
	{
		SpawnRangeAttackParticle(particleManager);
		m_RangeAttackCooldownTimer = m_RangeAttackCooldown;
	}
}

void Player::SpawnAttackParticle(ParticleManager* particleManager) const
{
	const float
		rotationAngle{ m_SplashSprite->GetRotation() },
		offset{ 30.f };

	Vector2f positionOffset{
		std::cosf(static_cast<float>(M_PI / 180.f) * rotationAngle)* offset,
		(GetSprite()->GetCurrentFrameDimensions().height) * 0.75f + std::sinf(static_cast<float>(M_PI / 180.f) * rotationAngle) * offset
	};

	particleManager->SpawnMelee(AttackParticle::OwnerType::Player, this, GetPosition(), positionOffset, m_BaseSplashHitbox, 0.21f, rotationAngle, m_SplashSprite->IsFlippedHorizontally(), m_SplashSprite->IsFlippedVertically());
}

void Player::SpawnRangeAttackParticle(ParticleManager* particleManager) const
{
	const float
		rotationAngle{ m_SplashSprite->GetRotation() },
		offset{ 30.f };

	Vector2f
		positionCorrection{ 0.f, GetCurrentHitbox().height * 0.5f };

	float angleRad{
		m_SplashSprite->GetRotation() * (static_cast<float>(M_PI) / 180.f)
	};

	Vector2f projectileVelocity{
		std::cosf(angleRad) * m_RangeProjectileSpeed ,
		std::sinf(angleRad) * m_RangeProjectileSpeed
	};

	particleManager->SpawnBullet(
		AttackParticle::OwnerType::Player,
		GetPosition() + positionCorrection,
		Vector2f{ 0.f, 0.f },
		m_BaseRangeRangeProjectileHitbox,
		projectileVelocity,
		angleRad,
		m_SplashSprite->IsFlippedHorizontally(),
		m_SplashSprite->IsFlippedVertically(),
		m_RangeProjectileSprite
	);
}

void Player::SpawnThrownObject(ParticleManager* particleManager, const Vector2f& mousePos) const
{
	const float
		rotationAngle{ static_cast<float>(M_PI / 180.f) * CalculateSplashRotation(mousePos) };

	const float
		throwSpeed{ 1200.f };

	Vector2f objectVelocity{ 
		std::cosf(rotationAngle) * throwSpeed ,
		std::sinf(rotationAngle) * throwSpeed
	};

	Vector2f
		positionCorrection{ 0.f, GetCurrentHitbox().height * 0.5f };
	// TODO globally fix all position so i dont need to write this in similar cases

	particleManager->SpawnThrownObject(
		GetPosition() + positionCorrection,
		objectVelocity,
		m_pHeldObject->GetSprite()
	);
}

void Player::AttackDash(const Vector2f& mousePos)
{
	SetIsOnGroundState(false);
	m_AirAttackCount++;

	Vector2f
		playerPos{ GetPosition() };

	Vector2f
		dashDirection{ (mousePos - playerPos).Normalized() };

	float
		decayFactor{ 0.2f };

	Vector2f
		finalDashVelocity{ m_BaseDashForce * dashDirection };
	finalDashVelocity.y += GetVelocityY();
	//finalDashVelocity.y *= std::powf(decayFactor, static_cast<float>(m_AirAttackCount - 1)); //decaying y velocity to prevent flying

	SetVelocity(finalDashVelocity);
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
	if (rollIntent && m_RollCooldownTimer <= 0)
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

float Player::CalculateSplashRotation(const Vector2f& mouseVec) const
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

void Player::UpdateAttackState(float elapsedSec)
{
	int
		renderSplashAfterFrame{ 2 };
	if (GetSprite()->GetCurrentFrameCount() >= renderSplashAfterFrame)
	{
		m_SplashSprite->SetVisible(true);
	}
	//UpdateSplashHitbox();
	ApplyAirResistance(elapsedSec);
}

void Player::UpdateCooldowns(float elapsedSec)
{
	m_AttackCooldownTimer -= elapsedSec;
	if (m_AttackCooldownTimer < 0.f)
	{
		m_AttackCooldownTimer = 0.f;
	}

	m_RollCooldownTimer -= elapsedSec;
	if (m_RollCooldownTimer < 0.f)
	{
		m_RollCooldownTimer = 0.f;
	}

	m_RangeAttackCooldownTimer -= elapsedSec;
	if (m_RangeAttackCooldownTimer < 0.f)
	{
		m_RangeAttackCooldownTimer = 0.f;
	}
}


