#pragma once
#include <string>

class Texture;

class Sprite final
{
public:
	struct CurrentAnimationState
	{

		Rectf frameDimension{};

		float frameTime{};
		int maxFrameCount{};
	};

	Sprite(const std::string& spritesheetTexturePath);

	Sprite(const Sprite&) = delete;
	Sprite& operator=(const Sprite&) = delete;

	~Sprite();

	void Draw(const Vector2f& drawPos, bool horizontalFlip, bool verticalFlip) const;

	void Update(float elapsedSec);

	void SetCurrentAnimationState(const Rectf& frameDimension, float frameTime, int maxFrameCount);
	void SetCurrentAnimationState(const CurrentAnimationState& animationState);

	bool IsFinished() const;
private:

	CurrentAnimationState m_CurrentAnimationState{};

	float m_AccumulatedTime{};
	int m_FrameCount{};

	bool m_LastFrameReached{};

	Texture* m_pSpritesheet;

};

