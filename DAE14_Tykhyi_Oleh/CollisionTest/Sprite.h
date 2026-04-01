#pragma once
#include "AnimationFrameInfo.h"
#include <string>

class Texture;

class Sprite final
{
public:
	Sprite(const std::string& spritesheetTexturePath);

	Sprite(const Sprite&) = delete;
	Sprite& operator=(const Sprite&) = delete;

	~Sprite();

	void Draw(const Vector2f& drawPos) const;

	void Update(float elapsedSec);

	void SetAnimationFrameInfo(const Rectf& frameDimension, float frameTime, int maxFrameCount);
	void SetAnimationFrameInfo(const AnimationFrameInfo& animationState);

	void RotateBy(float angle);

	void ResetRotation();

	void SetVisible(bool isVisible);
	void FlipHorizontally();
	void FlipVertically();

	void ResetHorizontalFlip();
	void ResetVerticalFlip();

	bool IsFinished() const;
	bool IsFlippedHorizontally() const;
	bool IsFlippedVertically() const;
	bool IsVisible() const;

	int GetCurrentFrameCount() const;

	void ResetAnimation();
private:
	AnimationFrameInfo m_AnimationFrameInfo{};

	float m_AccumulatedTime{};
	int m_FrameCount{};

	bool 
		m_IsFlippedHorizontally{},
		m_IsFlippedVertically{},
		m_LastFrameReached{},
		m_IsVisible{};

	float
		m_RotationAngle{};

	Texture* m_pSpritesheet;

};

