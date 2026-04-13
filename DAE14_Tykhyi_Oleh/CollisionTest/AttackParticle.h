#pragma once

#include <vector>

class Entity;

class AttackParticle final
{
public:
	enum class OwnerType {
		none = -1,

		Enemy,
		Player
	};

	enum class AttackType {
		none = -1,

		bullet,
		melee
	};

	AttackParticle() = default;

	void Spawn(
		OwnerType ownerType,
		AttackType attackType,
		const Vector2f& position,
		const Vector2f& positionOffset,
		const Vector2f& velocity,
		const std::vector<Vector2f>& localPoints,
		float lifetime,
		float rotationAngle,
		bool isFlippedHorizontally,
		bool isFlippedVertically, 
		const Entity* pOwnerEntity
	);

	void Draw() const;

	void Update(float elapsedSec);

	void Deactivate();

	void Deflect();

	bool IsActive() const;
	OwnerType GetOwnerType() const;
	AttackType GetAttackType() const;
	const std::vector<Vector2f>& GetWorldCoordinates() const;
private:
	OwnerType m_OwnerType{ OwnerType::none };
	AttackType m_AttackType{ AttackType::none };

	bool
		m_IsActive{ false },
		m_IsFlippedVertically{ false },
		m_IsFlippedHorizontally{ false};

	float
		m_Lifetime{},
		m_RotationAngle{};

	Vector2f
		m_Position{},
		m_PositionOffset{}, // like alternative to archoring point
		m_Velocity{};

	const Entity*
		m_pOwnerEntity{};

	std::vector<Vector2f> m_LocalHitbox{};
	std::vector<Vector2f> m_GlobalHitbox{};

	void UpdateHitboxGeometry();
};

