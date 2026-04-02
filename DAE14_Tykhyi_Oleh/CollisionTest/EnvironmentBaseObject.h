#pragma once

//TODO maybe i should consider using structs as i wont probably have 
// some special behavior on set and get methods
class EnvironmentBaseObject {
public:
	enum class EnvironmentObjectType {
		base,
		stairs
	};

	EnvironmentBaseObject(
		float positionX,
		float positionY, 
		const Rectf& firstCollider, 
		const std::string& texturePath
	);

	virtual ~EnvironmentBaseObject() = default;

	EnvironmentObjectType GetType() const;

	const Rectf& GetFirstCollider() const;

protected:
	void SetType(EnvironmentObjectType type);
private:
	Vector2f m_Position;

	Rectf m_FirstCollider;

	std::string m_TexturePath; //TODO change it to pointer to sprite(not shared for now but it will be shared by)

	EnvironmentObjectType m_Type;
};