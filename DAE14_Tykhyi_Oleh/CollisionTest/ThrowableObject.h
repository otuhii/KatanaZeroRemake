#pragma once
#include "InteractableObject.h"
#include "Sprite.h"

class ThrowableObject :
    public InteractableObject
{
public:
    ThrowableObject(Sprite* pSprite, const Vector2f& position, float interactionRange, float scale, int floor);

    void Draw() const override;
    void Interact(SoundManager* pSoundManager) override;

    Sprite* GetSprite() const;

private:
    Sprite* m_pSprite{};
};

