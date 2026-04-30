#pragma once
#include "AnimationFrameInfo.h"
#include <string>

class Texture;

class Sprite final
{
public:
	Sprite(const std::string& spritesheetTexturePath);
	Sprite(const Sprite& other);

	/*Sprite(const Sprite&) = delete;
	Sprite& operator=(const Sprite&) = delete;*/
	~Sprite();

	void Draw(const Vector2f& drawPos, bool pivotX, bool pivotY) const;

	void Update(float elapsedSec);

	void SetAnimationFrameInfo(const Rectf& frameDimension, float frameTime, int maxFrameCount);
	void SetAnimationFrameInfo(const AnimationFrameInfo& animationState);

	void RotateBy(float angle);

	void ResetRotation();

	void SetScale(float scale);

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
	const Rectf& GetCurrentFrameDimensions() const;
	int GetCurrentMaxFrameCount() const;

	float GetScale() const;
	float GetRotation() const;

	void ResetAnimation();

	void SetLooping(bool isLooping);
	void SetStatic(bool isStatic);
private:
	AnimationFrameInfo m_AnimationFrameInfo{};

	float m_AccumulatedTime{};
	int m_FrameCount{};

	bool
		m_IsFlippedHorizontally{ false },
		m_IsFlippedVertically{ false },
		m_LastFrameReached{ false },
		m_IsVisible{ false },
		m_IsLooping{ true },
		m_IsStatic{ false };

	float
		m_RotationAngle{},
		m_ScaleValue{};

	Texture* m_pSpritesheet;


	bool
		m_OwnsTexture{ true };
};

