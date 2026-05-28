#pragma once
#include "InteractableObject.h"

class SoundManager;
class Sprite;

class ThrowableObject final:
    public InteractableObject
{
public:
    explicit ThrowableObject(Sprite* pSprite, const Vector2f& position, float interactionRange, float scale, int floor);

    void Draw() const override;
    void Interact(SoundManager* pSoundManager) override;

    void Reset() override;

    Sprite* GetSprite() const;
private:
    Sprite* m_pSprite{};
};

