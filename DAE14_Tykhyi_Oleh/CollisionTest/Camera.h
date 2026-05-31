#pragma once

class Camera
{
public:
	explicit Camera(float screenWidth, float screenHeight);

	void Update(
		float elapsedSec, 
		const Vector2f& mouseScreenPos,
		const Vector2f& trackCenter,
		float levelWidth, 
		float levelHeight
	);

	void Aim() const;

	void Reset();

	const Vector2f& GetBasePoint() const;
private:
	const float
		m_ScreenWidth{},
		m_ScreenHeight{},
		m_CameraTrackSpeed{ 10.f },
		m_MouseInfluence{ 0.15f };

	Vector2f
		m_CurrentCenter{},
		m_BasePoint{};

	bool
		m_IsInitialized{ true };
};