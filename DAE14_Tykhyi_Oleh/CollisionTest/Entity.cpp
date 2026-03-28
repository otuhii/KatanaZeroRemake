#include "pch.h"
#include "Entity.h"

Entity::Entity(Sprite* sprite)
	: m_pSprite{sprite}
{

}

Sprite* Entity::GetSprite() const
{
	return m_pSprite;
}
