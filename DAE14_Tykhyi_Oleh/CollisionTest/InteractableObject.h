#pragma once

class SoundManager;
class Player;

struct InteractableObjectSnapshot;

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


	explicit InteractableObject(const Vector2f& position, int floor, float interactionRange, InteractableType type);

	virtual ~InteractableObject() = default;

	InteractableObject(const InteractableObject&) = delete;
	InteractableObject& operator=(const InteractableObject&) = delete;
	InteractableObject(InteractableObject&&) = delete;
	InteractableObject& operator=(InteractableObject&&) = delete;

	virtual void Draw() const = 0;

	virtual void Update(float elapsedSec, SoundManager* pSoundManager, Player* pPlayer);

	virtual void Interact(SoundManager* pSoundManager) = 0;

	virtual void Reset();

	virtual void SaveSnapshot(InteractableObjectSnapshot& snapshot);
	virtual void ApplySnapshot(const InteractableObjectSnapshot& snapshot);

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
