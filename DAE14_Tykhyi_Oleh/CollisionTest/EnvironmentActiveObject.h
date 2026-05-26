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
		jumpThroughPlatform,
		endTrigger
	};

	EnvironmentActiveObject(
		float positionX,
		float positionY,
		const std::vector<Rectf>& colliders,
		const Sprite* pTexture,
		EnvironmentObjectType type,
		int floor
	);

	const std::vector<Rectf>& GetColliders() const;

	const Vector2f& GetPosition() const;
	const Sprite*   GetTexture() const;

	EnvironmentObjectType GetType() const;
	int GetFloor() const;
private:
	Vector2f m_Position;

	std::vector<Rectf> m_Colliders{};

	int m_Floor{};

	const Sprite* m_pTexture{};

	EnvironmentObjectType m_Type{EnvironmentObjectType::none};
};

