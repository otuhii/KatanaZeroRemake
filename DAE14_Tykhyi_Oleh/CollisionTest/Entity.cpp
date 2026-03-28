#include "pch.h"
#include "Entity.h"

Entity::Entity(Sprite* sprite)
	: m_pSprite{sprite}
{

}

//void Entity::Update(float elapsedSec)
//{
//
//}

Sprite* Entity::GetSprite() const
{
	return m_pSprite;
}
