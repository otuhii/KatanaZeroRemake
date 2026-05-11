#include "pch.h"
#include "InteractableObject.h"

InteractableObject::InteractableObject(const Vector2f& position, int floor, float interactionRange, InteractableType type)
    : m_Position{ position }, m_Floor{ floor }, m_InteractionRange{ interactionRange }, m_Type{ type }
{
}

void InteractableObject::Update(float elapsedSec, SoundManager* pSoundManager)
{

}

bool InteractableObject::IsPlayerInRange(const Vector2f& playerPos, int playerFloor) const
{
    return ((playerPos - m_Position).SquaredLength() <= m_InteractionRange * m_InteractionRange);
}

const Vector2f& InteractableObject::GetPosition() const
{
    return m_Position;
}

void InteractableObject::SetInactive()
{
    m_IsActive = false;
}

bool InteractableObject::IsActive() const
{
    return m_IsActive;
}

InteractableObject::InteractableType InteractableObject::GetType() const
{
    return m_Type;
}


int InteractableObject::GetFloor() const
{
    return m_Floor;
}
