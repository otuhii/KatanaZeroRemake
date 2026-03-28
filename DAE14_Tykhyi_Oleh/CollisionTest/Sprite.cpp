#include "pch.h"
#include "Sprite.h"
#include "Texture.h"

Sprite::Sprite(const int rows, const int cols, const std::string& spritesheetTexturePath)
	: m_Rows{rows}, m_Cols{cols}, m_AccumulatedTime{0.f}, m_FrameCount{0}
{
	m_pSpritesheet = new Texture{ spritesheetTexturePath };
}

Sprite::~Sprite()
{
	delete m_pSpritesheet;
}

void Sprite::SetCurrentAnimationState(const Rectf& frameDimension, float frameTime, int maxFrameCount)
{
	m_CurrentAnimationState.frameTime = frameTime;
	m_CurrentAnimationState.frameDimension = frameDimension;
	m_CurrentAnimationState.maxFrameCount = maxFrameCount;
}

void Sprite::SetCurrentAnimationState(const CurrentAnimationState & animationState)
{
	m_CurrentAnimationState = animationState;
}

void Sprite::Draw(const Vector2f& drawPos) const
{
	glPushMatrix();
	{
		glTranslatef(drawPos.x, drawPos.y, 0.f);
		glScalef(2.f, 2.f, 1.f);

		Rectf srcRect{
			m_CurrentAnimationState.frameDimension.left+m_CurrentAnimationState.frameDimension.width * m_FrameCount, // TODO i think i can update this shit in the update function
			m_CurrentAnimationState.frameDimension.bottom,
			m_CurrentAnimationState.frameDimension.width,
			m_CurrentAnimationState.frameDimension.height
		};

		m_pSpritesheet->Draw(drawPos, srcRect);
	}
	glPopMatrix();
}

void Sprite::Update(float elapsedSec)
{
	m_AccumulatedTime += elapsedSec;

	if (m_AccumulatedTime > m_CurrentAnimationState.frameTime)
	{
		m_AccumulatedTime -= m_CurrentAnimationState.frameTime;
		m_FrameCount = (m_FrameCount + 1) % m_CurrentAnimationState.maxFrameCount;
	}
}

