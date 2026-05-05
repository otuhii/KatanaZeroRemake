#pragma once


class CosmeticParticle final
{ 
public:
	enum class CosmeticParticleType {
		none = -1,

		dust,
		blood,

		count
	};

	void Spawn(
		const Vector2f& position,
		const Vector2f& velocity,
		float lifetime,
		bool applyGravity
	);

	void Draw() const;

	void Update(float elapsedSec);

	void Deactivate();

	bool IsActive() const;

private:
	

	Vector2f
		m_Position{},
		m_Velocity{};

	float
		m_Lifetime{};

	bool
		m_ApplyGravity{},
		m_IsActive{};

};

