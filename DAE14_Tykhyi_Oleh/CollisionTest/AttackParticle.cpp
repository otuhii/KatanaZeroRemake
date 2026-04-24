#include "pch.h"
#include "AttackParticle.h"

#include "Entity.h"
#include "Matrix2x3.h"
#include "utils.h"

void AttackParticle::Spawn(OwnerType ownerType, AttackType attackType, const Vector2f& position, const Vector2f& positionOffset, const Vector2f& velocity, const std::vector<Vector2f>& localPoints, float lifetime, float rotationAngle,  bool isFlippedHorizontally, bool isFlippedVertically, const Entity* pOwnerEntity)
{
	m_OwnerType = ownerType;
	m_AttackType = attackType;
	m_Position = position;
	m_PositionOffset = positionOffset;
	m_Velocity = velocity;

	m_IsFlippedVertically = isFlippedVertically;
	m_IsFlippedHorizontally = isFlippedHorizontally;

	m_RotationAngle = rotationAngle;

	m_LocalHitbox = localPoints;

	m_Lifetime = lifetime;
	m_pOwnerEntity = pOwnerEntity;

	m_IsActive = true;

	UpdateHitboxGeometry();
}

void AttackParticle::Draw() const
{
	if (!m_IsActive || m_GlobalHitbox.empty())
	{
		return;
	}

	if (m_OwnerType == OwnerType::Player)
	{
		utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	}
	else
	{
		utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	}

	utils::DrawPolygon(m_GlobalHitbox);
}

void AttackParticle::Update(float elapsedSec)
{
	if (!m_IsActive)
	{
		return;
	}

	m_Lifetime -= elapsedSec;

	if (m_Lifetime <= 0.f)
	{
		Deactivate();
		return;
	}

	if (m_AttackType == AttackType::melee && m_pOwnerEntity != nullptr)
	{
		m_Position = m_pOwnerEntity->GetPosition();
	}
	else
	{
		m_Position.x += m_Velocity.x * elapsedSec;
		m_Position.y += m_Velocity.y * elapsedSec;
	}

	UpdateHitboxGeometry();
}

void AttackParticle::Deactivate()
{
	m_IsActive = false;
	m_pOwnerEntity = nullptr;
	m_LocalHitbox.clear();
	m_GlobalHitbox.clear();
	m_AttackType = AttackType::none;
	m_OwnerType = OwnerType::none;
}

void AttackParticle::Deflect()
{
	float
		deflectMultiplier{ 1.5f };

	if (m_AttackType == AttackType::bullet)
	{
		m_OwnerType = OwnerType::Player;
		m_Velocity.x = -m_Velocity.x * deflectMultiplier;
		m_Velocity.y = -m_Velocity.y * deflectMultiplier;
	}
}

bool AttackParticle::IsActive() const
{
	return m_IsActive;
}

AttackParticle::OwnerType AttackParticle::GetOwnerType() const
{
	return m_OwnerType;
}

AttackParticle::AttackType AttackParticle::GetAttackType() const
{
	return m_AttackType;
}

const std::vector<Vector2f>& AttackParticle::GetWorldCoordinates() const
{
	return m_GlobalHitbox;
}

const Vector2f& AttackParticle::GetVelocity() const
{
	return m_Velocity;
}

const Entity* AttackParticle::GetOwnerEntity() const
{
	return m_pOwnerEntity;
}

void AttackParticle::UpdateHitboxGeometry()
{
	if (m_LocalHitbox.empty())
	{
		return;
	}

	Matrix2x3
		hitboxRotation{},
		hitboxTranslation{},
		hitboxFlip{},
		transformation{};

	float scaleX{1};
	if (m_IsFlippedHorizontally)
	{
		scaleX = -1;
	}
	float scaleY{ 1 };
	if (m_IsFlippedVertically)
	{
		scaleY = -1;
	}

	hitboxRotation.SetAsRotate(m_RotationAngle);
	hitboxFlip.SetAsScale(scaleX, scaleY);
	hitboxTranslation.SetAsTranslate(m_Position+m_PositionOffset);

	if (m_IsFlippedHorizontally || m_IsFlippedVertically)
	{
		transformation = hitboxTranslation * hitboxRotation * hitboxFlip;
	}
	else
	{
		transformation = hitboxTranslation * hitboxRotation;
	}

	m_GlobalHitbox = transformation.Transform(m_LocalHitbox);
}