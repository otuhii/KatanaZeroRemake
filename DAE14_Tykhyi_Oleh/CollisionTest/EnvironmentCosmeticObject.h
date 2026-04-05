#pragma once
#include "Sprite.h"

class EnvironmentCosmeticObject final
{
public:
	EnvironmentCosmeticObject(
		float positionX,
		float positionY,
		const Sprite* pTexture
	);

	const Vector2f& GetPosition() const;
	const Sprite*	GetTexture() const;
private:
	Vector2f m_Position{};

	const Sprite* m_pTexture{};
};

