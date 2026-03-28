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

	Sprite(int rows, int cols, const std::string& spritesheetTexturePath);

	~Sprite();

	void Draw(const Vector2f& drawPos) const;

	void Update(float elapsedSec);

	void SetCurrentAnimationState(const Rectf& frameDimension, float frameTime, int maxFrameCount);
	void SetCurrentAnimationState(const CurrentAnimationState& animationState);

private:

	const int m_Rows{};
	const int m_Cols{};

	CurrentAnimationState m_CurrentAnimationState{};

	float m_AccumulatedTime{};
	int m_FrameCount{};

	Texture* m_pSpritesheet;

};

