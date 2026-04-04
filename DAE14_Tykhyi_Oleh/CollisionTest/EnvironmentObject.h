#pragma once
#include <vector>

class EnvironmentObject final
{
public:
	enum class EnvironmentObjectType {
		platform,
		stairs
	};

	EnvironmentObject(
		float positionX,
		float positionY,
		const std::vector<Rectf>& colliders,
		const std::string& texturePath
	);



	const std::vector<Rectf>& GetColliders() const;

	const Vector2f& GetPosition() const;
private:
	Vector2f m_Position;
	std::string m_TexturePath; 

	std::vector<Rectf> m_Colliders{};

	//int textureId
};

