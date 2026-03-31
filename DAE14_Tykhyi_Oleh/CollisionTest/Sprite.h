#pragma once
#include "CurrentAnimationState.h"
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

	void SetCurrentAnimationState(const Rectf& frameDimension, float frameTime, int maxFrameCount);
	void SetCurrentAnimationState(const CurrentAnimationState& animationState);

	void FlipHorizontally();
	void FlipVertically();

	void ResetHorizontalFlip();
	void ResetVerticalFlip();

	bool IsFinished() const;
	bool IsFlippedHorizontally() const;
	bool IsFlippedVertically() const;
private:

	CurrentAnimationState m_CurrentAnimationState{};

	float m_AccumulatedTime{};
	int m_FrameCount{};

	bool m_LastFrameReached{};

	bool 
		m_IsFlippedHorizontally{},
		m_IsFlippedVertically{};


	Texture* m_pSpritesheet;

};

