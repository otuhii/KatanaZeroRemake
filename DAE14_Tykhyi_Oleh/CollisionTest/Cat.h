#pragma once
#include "InteractableObject.h"
#include "Sprite.h"

class Cat final:
    public InteractableObject
{
public:
	enum class CatState
	{
		toLick,
		petting,
		licking,
		idle,
		fromLick,
		lookBack
	};

	Cat(Sprite* pSprite, const std::vector<AnimationFrameInfo>& catAnimation, const Vector2f& position, int floor, float interactionRange, float scale);

	void Draw() const override;

	void Update(float elapsedSec, SoundManager* pSoundManager, Player* pPlayer) override;

	void SetState(CatState state, SoundManager* pSoundManager);

	void Interact(SoundManager* pSoundManager) override;

	void SaveSnapshot(InteractableObjectSnapshot& snapshot) override;
	void ApplySnapshot(const InteractableObjectSnapshot& snapshot) override;
private:
	Sprite* m_pSprite{};

	CatState m_State{};

	std::vector<AnimationFrameInfo> m_CatSpriteFrames{};

	float
		m_IdleTimer{ 0.f },
		m_IdleDuration{ 0.f };

	int
		m_LickCycles{ 0 },
		m_TargetLickCycles{ 1 };

};

