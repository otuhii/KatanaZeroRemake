#pragma once

class SpriteManager;
class Sprite;

class CosmeticParticle final
{ 
public:
	CosmeticParticle(SpriteManager* pSpriteManager);

	void Spawn(
		const Vector2f& position,
		const Vector2f& velocity,
		float lifetime,
		bool applyGravity,
		Sprite* pTemplateSprite
	);

	void Draw() const;

	void Update(float elapsedSec);

	void Deactivate();

	bool IsActive() const;

private:
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

