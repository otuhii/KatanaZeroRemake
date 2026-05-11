#include "pch.h"
#include "CosmeticParticle.h"
#include "SpriteManager.h"
#include "Sprite.h"

CosmeticParticle::CosmeticParticle(SpriteManager* pSpriteManager)
{
	m_pSprite = pSpriteManager->CreateEmptySprite();
}

void CosmeticParticle::Spawn(
	CosmeticParticleType type,
	const Vector2f& position,
	const Vector2f& velocity, 
	float lifetime, 
	bool applyGravity,
	float spriteRotation,
	Sprite* pTemplateSprite,
	float scale
)
{
	m_pSprite->CopyFrom(pTemplateSprite);
	m_pSprite->RotateBy(spriteRotation);

	m_pSprite->SetScale(scale);

	m_Lifetime = lifetime;
	m_Position = position;
	m_Velocity = velocity;
	m_ApplyGravity = applyGravity;
	m_IsActive = true;
}

void CosmeticParticle::Draw() const
{
	if (m_pSprite)
	{
		m_pSprite->Draw(m_Position, true, true);
	}
}

void CosmeticParticle::Update(float elapsedSec)
{
	const float
		gravity{ -981.f };

	m_Lifetime -= elapsedSec;

	if (m_Lifetime <= 0.f)
	{
		Deactivate();
		return;
	}

	if (m_ApplyGravity)
	{
		m_Velocity.y += elapsedSec * gravity;
	}

	m_Position += elapsedSec * m_Velocity;
}

void CosmeticParticle::Deactivate()
{
	m_IsActive = false;
}

bool CosmeticParticle::IsActive() const
{
	return m_IsActive;
}

void CosmeticParticle::SetRandomFrame()
{
	int
		maxFrame{ m_pSprite->GetCurrentMaxFrameCount() };

	int 
		randFrame{ rand() % maxFrame };

	m_pSprite->SetCurrentFrame(randFrame);
}
