#pragma once
#include "Sprite.h"


class Entity
{
public:
	Entity(Sprite* sprite);

	virtual void Draw(const Vector2f& drawPos) const = 0;

	/*virtual void Update(float elapsedSec);*/

protected:
	Sprite* GetSprite() const;

private:
	Sprite* m_pSprite;

};

