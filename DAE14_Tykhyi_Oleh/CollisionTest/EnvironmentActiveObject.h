#pragma once
#include <vector>
#include "Sprite.h"

class EnvironmentActiveObject final
{
public:
	enum class EnvironmentObjectType {
		none=-1,

		platform,
		stairs,
		jumpThroughPlatform
	};

	EnvironmentActiveObject(
		float positionX,
		float positionY,
		const std::vector<Rectf>& colliders,
		const Sprite* pTexture,
		EnvironmentObjectType type
	);

	const std::vector<Rectf>& GetColliders() const;

	const Vector2f& GetPosition() const;
	const Sprite*   GetTexture() const;

	EnvironmentObjectType GetType() const;
private:
	Vector2f m_Position;

	std::vector<Rectf> m_Colliders{};

	const Sprite* m_pTexture{};

	EnvironmentObjectType m_Type{EnvironmentObjectType::none};
	//int textureId
};

