#include "pch.h"
#include "Sprite.h"
#include "Texture.h"

Sprite::Sprite(const std::string& spritesheetTexturePath)
	: m_AccumulatedTime{0.f}, m_FrameCount{0}
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

bool Sprite::IsFinished() const
{
	return m_LastFrameReached;
}

void Sprite::Draw(const Vector2f& drawPos, bool horizontalFlip, bool verticalFlip) const
{
	glPushMatrix();
	{
		glTranslatef(drawPos.x, drawPos.y, 0.f);
		glScalef(3.f, 3.f, 1.f);

		if (horizontalFlip)
		{
			float halfWidth{
				m_CurrentAnimationState.frameDimension.width * 0.5f
			};
			glTranslatef(halfWidth, 0.f, 0.f);

			glScalef(-1.f, 1.f, 1.f);

			glTranslatef(-halfWidth, 0.f, 0.f);
		}

		if (verticalFlip)
		{
			float halfHeight{
				m_CurrentAnimationState.frameDimension.height * 0.5f
			};
			glTranslatef(0.f, halfHeight, 0.f);

			glScalef(1.f, -1.f, 1.f);

			glTranslatef(0.f, -halfHeight, 0.f);
		}

		Rectf srcRect{
			m_CurrentAnimationState.frameDimension.left+m_CurrentAnimationState.frameDimension.width * m_FrameCount,
			m_CurrentAnimationState.frameDimension.bottom,
			m_CurrentAnimationState.frameDimension.width,
			m_CurrentAnimationState.frameDimension.height
		};

		m_pSpritesheet->Draw(Vector2f{}, srcRect);
	}
	glPopMatrix();
}

void Sprite::Update(float elapsedSec)
{
	m_LastFrameReached = false;

	m_AccumulatedTime += elapsedSec;

	if (m_AccumulatedTime > m_CurrentAnimationState.frameTime)
	{
		m_AccumulatedTime -= m_CurrentAnimationState.frameTime;

		if (m_FrameCount == m_CurrentAnimationState.maxFrameCount - 1)
		{
			m_LastFrameReached = true;
		}

		m_FrameCount = (m_FrameCount + 1) % m_CurrentAnimationState.maxFrameCount;
	}
}

