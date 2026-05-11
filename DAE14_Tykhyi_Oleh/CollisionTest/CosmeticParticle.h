#pragma once

class SpriteManager;
class Sprite;

class CosmeticParticle final
{ 
public:
	enum class CosmeticParticleType {
		none=-1,

		dust,
		blood,
		bloodSlash,
		cracks,

		count
	};

	CosmeticParticle(SpriteManager* pSpriteManager);

	void Spawn(
		CosmeticParticleType type,
		const Vector2f& position,
		const Vector2f& velocity,
		float lifetime,
		bool applyGravity,
		float spriteRotation,
		Sprite* pTemplateSprite,
		float scale
	);

	void Draw() const;

	void Update(float elapsedSec);

	void Deactivate();

	bool IsActive() const;

	void SetRandomFrame();
private:
	CosmeticParticleType m_Type{ CosmeticParticleType::none };

	Sprite*
		m_pSprite;

	Vector2f
		m_Position{},
		m_Velocity{};

	float
		m_Lifetime{};

	bool
		m_ApplyGravity{},
		m_IsActive{};

};

