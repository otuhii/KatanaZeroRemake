#include "pch.h"
#include "Camera.h"

Camera::Camera(float screenWidth, float screenHeight)
	: m_ScreenWidth{ screenWidth }, m_ScreenHeight{ screenHeight }
{
}

void Camera::Aim(float levelW, float levelH, const Vector2f& trackCenter)
{
	m_BasePoint.x = trackCenter.x - m_ScreenWidth * 0.5f;
	m_BasePoint.y = trackCenter.y - m_ScreenHeight * 0.5f;

	if (m_BasePoint.x < 0.f)
	{
		m_BasePoint.x = 0.f;
	}
	if (m_BasePoint.y < 0.f)
	{
		m_BasePoint.y = 0.f;
	}

	if (m_BasePoint.x > levelW-m_ScreenWidth)
	{
		m_BasePoint.x = levelW - m_ScreenWidth;
	}
	if (m_BasePoint.y > levelH-m_ScreenHeight)
	{
		m_BasePoint.y = levelH - m_ScreenHeight;
	}



	glPushMatrix();
	glTranslatef(-m_BasePoint.x, -m_BasePoint.y, 0.f);
}

void Camera::Reset()
{
	glPopMatrix();
}

const Vector2f& Camera::GetBasePoint() const
{
	return m_BasePoint;
}
