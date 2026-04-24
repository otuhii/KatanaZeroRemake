#include "pch.h"
#include "Camera.h"


Camera::Camera(float screenWidth, float screenHeight)
	: m_ScreenWidth{ screenWidth }, m_ScreenHeight{ screenHeight }
{
}

void Camera::Update(
	float elapsedSec, 
	const Vector2f& trackCenter, 
	float levelWidth, 
	float levelHeight
)
{
	//current = current + (target - current) * elapsedSec * speed
	if (!m_IsInitialized)
	{
		m_CurrentCenter = trackCenter;
		m_IsInitialized = true;
	}

	m_CurrentCenter += (trackCenter - m_CurrentCenter) * m_CameraTrackSpeed * elapsedSec;
	
	m_BasePoint.x = m_CurrentCenter.x - m_ScreenWidth * 0.5f;
	m_BasePoint.y = m_CurrentCenter.y - m_ScreenHeight * 0.5f;

	if (m_BasePoint.x > levelWidth - m_ScreenWidth)
	{
		m_BasePoint.x = levelWidth - m_ScreenWidth;
	}
	else if (m_BasePoint.x < 0.f)
	{
		m_BasePoint.x = 0.f;
	}

	if (m_BasePoint.y > levelHeight - m_ScreenHeight)
	{
		m_BasePoint.y = levelHeight - m_ScreenHeight;
	}
	else if (m_BasePoint.y < 0.f)
	{
		m_BasePoint.y = 0.f;
	}
}

void Camera::Aim()
{

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
