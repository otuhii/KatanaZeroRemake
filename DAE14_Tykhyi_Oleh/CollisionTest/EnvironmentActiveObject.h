#pragma once
#include <vector>
#include "Sprite.h"

class EnvironmentActiveObject final
{
public:
	enum class EnvironmentObjectType {
		platform,
		stairs
	};

	EnvironmentActiveObject(
		float positionX,
		float positionY,
		const std::vector<Rectf>& colliders,
		const Sprite* pTexture
	);

	const std::vector<Rectf>& GetColliders() const;

	const Vector2f& GetPosition() const;
	const Sprite*   GetTexture() const;
private:
	Vector2f m_Position;

	std::vector<Rectf> m_Colliders{};

	const Sprite* m_pTexture{};

	//int textureId
};

