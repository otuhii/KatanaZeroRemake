#include "pch.h"
#include "MoveSemantics.h"
#include "Texture.h"

MoveSemantics::~MoveSemantics()
{
	delete m_pTexture;
}

MoveSemantics::MoveSemantics(MoveSemantics&& rhs)
	: m_SomeTextForMoving{std::move(rhs.m_SomeTextForMoving)},
	m_pTexture{std::move(rhs.m_pTexture)}
{
	rhs.m_pTexture = nullptr;
}

MoveSemantics& MoveSemantics::operator=(MoveSemantics&& rhs)
{
	if (&rhs != this)
	{
		delete this->m_pTexture;

		this->m_SomeTextForMoving = std::move(rhs.m_SomeTextForMoving);
		this->m_pTexture = std::move(rhs.m_pTexture);
		rhs.m_pTexture = nullptr;
	}

	return *this;
}
