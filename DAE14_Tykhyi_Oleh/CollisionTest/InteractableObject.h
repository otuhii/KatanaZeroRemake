#pragma once

class InteractableObject
{
public:
	enum class InteractableType
	{
		cat,
		door,
		throwableObject,
		light
	};


	InteractableObject(const Vector2f& position, int floor, float interactionRange, InteractableType type);

	virtual ~InteractableObject() = default;

	virtual void Draw() const = 0;

	virtual void Update(float elapsedSec);

	virtual void Interact() = 0;

	bool IsPlayerInRange(const Vector2f& playerPos, int playerFloor) const;

	const Vector2f&		GetPosition() const;
	InteractableType	GetType() const;
	int					GetFloor() const;

	void SetInactive();
	bool IsActive() const;
private:
	InteractableType
		m_Type{};

	bool 
		m_IsActive{true};

	float
		m_InteractionRange{};
	Vector2f
		m_Position{};
	int
		m_Floor{};
};
