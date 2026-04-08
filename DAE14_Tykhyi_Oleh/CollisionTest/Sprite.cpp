#include "pch.h"
#include "Sprite.h"
#include "Texture.h"

#include <iostream>

Sprite::Sprite(const std::string& spritesheetTexturePath)
	: m_AccumulatedTime{ 0.f }, m_FrameCount{ 0 }, m_IsVisible{ true }, m_ScaleValue{ 1.f }
{
	m_pSpritesheet = new Texture{ spritesheetTexturePath };

	m_AnimationFrameInfo.frameDimension.width = m_pSpritesheet->GetWidth();
	m_AnimationFrameInfo.frameDimension.height = m_pSpritesheet->GetHeight();
	m_AnimationFrameInfo.maxFrameCount = 1;
	m_AnimationFrameInfo.frameTime = 1.f;
}

Sprite::~Sprite()
{
	delete m_pSpritesheet;
}

void Sprite::SetAnimationFrameInfo(const Rectf& frameDimension, float frameTime, int maxFrameCount)
{
	m_AnimationFrameInfo.frameTime = frameTime;
	m_AnimationFrameInfo.frameDimension = frameDimension;
	m_AnimationFrameInfo.maxFrameCount = maxFrameCount;
}

void Sprite::SetAnimationFrameInfo(const AnimationFrameInfo & animationState)
{
	m_AnimationFrameInfo = animationState;
}

void Sprite::RotateBy(float angle)
{
	m_RotationAngle = angle;
}

void Sprite::ResetRotation()
{
	m_RotationAngle = 0.f;
}

void Sprite::SetScale(float scale)
{
	m_ScaleValue = scale;
}

void Sprite::SetVisible(bool isVisible)
{
	m_IsVisible = isVisible;
}

void Sprite::FlipHorizontally()
{
	m_IsFlippedHorizontally = true;
}

void Sprite::FlipVertically()
{
	m_IsFlippedVertically = true;
}

void Sprite::ResetHorizontalFlip()
{
	m_IsFlippedHorizontally = false;
}

void Sprite::ResetVerticalFlip()
{
	m_IsFlippedVertically = false;
}

bool Sprite::IsFinished() const
{
	return m_LastFrameReached;
}

bool Sprite::IsFlippedHorizontally() const
{
	return m_IsFlippedHorizontally;
}

bool Sprite::IsFlippedVertically() const
{
	return m_IsFlippedVertically;
}

bool Sprite::IsVisible() const
{
	return m_IsVisible;
}

int Sprite::GetCurrentFrameCount() const
{
	return m_FrameCount;
}

const Rectf& Sprite::GetCurrentFrameDimensions() const
{
	return m_AnimationFrameInfo.frameDimension;
}

float Sprite::GetScale() const
{
	return m_ScaleValue;
}

float Sprite::GetRotation() const
{
	return m_RotationAngle;
}

void Sprite::ResetAnimation()
{
	m_AccumulatedTime = 0.f;
	m_FrameCount = 0;
	m_LastFrameReached = false;
	m_RotationAngle = 0.f;
}

void Sprite::Draw(const Vector2f& drawPos, bool pivotX, bool pivotY) const
{
	if (!m_IsVisible) { return; }

	glPushMatrix();
	{
		Vector2f
			pivotPoint{ 0.f,0.f };

		float
			halfWidth{ m_AnimationFrameInfo.frameDimension.width * 0.5f },
			halfHeight{ m_AnimationFrameInfo.frameDimension.height * 0.5f };

		glTranslatef(drawPos.x, drawPos.y, 0.f);
		glRotatef(m_RotationAngle, 0.f, 0.f, 1.f);
		glScalef(m_ScaleValue, m_ScaleValue, 1.f);
		
		if (pivotX)
		{
			pivotPoint.x = -halfWidth;
		}
		if (pivotY)
		{
			pivotPoint.y = -halfHeight;
		}

		glTranslatef(pivotPoint.x, pivotPoint.y, 0.f);

		if (m_IsFlippedHorizontally)
		{
			
			glTranslatef(halfWidth, 0.f, 0.f);

			glScalef(-1.f, 1.f, 1.f);

			glTranslatef(-halfWidth, 0.f, 0.f);
		}

		if (m_IsFlippedVertically)
		{
			
			glTranslatef(0.f, halfHeight, 0.f);

			glScalef(1.f, -1.f, 1.f);

			glTranslatef(0.f, -halfHeight, 0.f);
		}

		Rectf srcRect{
			m_AnimationFrameInfo.frameDimension.left+m_AnimationFrameInfo.frameDimension.width * m_FrameCount,
			m_AnimationFrameInfo.frameDimension.bottom,
			m_AnimationFrameInfo.frameDimension.width,
			m_AnimationFrameInfo.frameDimension.height
		};

		m_pSpritesheet->Draw(Vector2f{}, srcRect);
	}
	glPopMatrix();
}

void Sprite::Update(float elapsedSec)
{
	if (!m_IsVisible) { return; }

	m_LastFrameReached = false;

	m_AccumulatedTime += elapsedSec;

	if (m_AccumulatedTime > m_AnimationFrameInfo.frameTime)
	{
		m_AccumulatedTime -= m_AnimationFrameInfo.frameTime;

		if (m_FrameCount == m_AnimationFrameInfo.maxFrameCount - 1)
		{
			m_LastFrameReached = true;
		}

		m_FrameCount = (m_FrameCount + 1) % m_AnimationFrameInfo.maxFrameCount;
	}
}

