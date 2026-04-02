#pragma once
#include "EnvironmentBaseObject.h"

class EnvironmentStairObject final : public EnvironmentBaseObject
{
public:
	EnvironmentStairObject(
		float positionX, 
		float positionY,
		const Rectf& firstCollider, 
		const Rectf& secondCollider,
		const std::string& texturePath
	);

	const Rectf& GetSecondCollider() const;

private:
	Rectf m_SecondCollider;
};

