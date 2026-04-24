#pragma once


class Camera
{
public:
	Camera(float screenWidth, float screenHeight);

	void Aim(float levelWidth, float levelHeight, const Vector2f& trackCenter);

	void Reset();

	const Vector2f& GetBasePoint() const;
private:
	float
		m_ScreenWidth{},
		m_ScreenHeight{};

	Vector2f
		m_BasePoint{};
};
