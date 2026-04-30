#pragma once
#include "InteractableObject.h"
#include "Sprite.h"

class Door final:
    public InteractableObject
{

public:
	Door(Sprite* pSprite, const std::vector<AnimationFrameInfo>& doorAnimation, const Vector2f& position, int floor, const Rectf& openedHitbox, const Rectf& closedHitbox, float scale);

	void Draw() const override;

	void Update(float elapsedSec) override;

	void Interact() override;

	bool IsOpened() const;
	bool IsOpening() const;

	const Rectf& GetCurrentCollider() const;
private:
	Sprite* m_pSprite{};

	bool
		m_IsOpening{},
		m_IsOpened{};

	std::vector<AnimationFrameInfo> m_SpriteFrames{};

	Rectf
		m_OpenedHitbox{},
		m_ClosedHitbox{};
};

