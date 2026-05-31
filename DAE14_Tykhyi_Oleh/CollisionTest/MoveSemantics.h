#pragma once
#include <string>

class Texture;

class MoveSemantics
{
public:
	MoveSemantics() = delete;

	~MoveSemantics();

	MoveSemantics(MoveSemantics&& rhs);
	MoveSemantics& operator=(MoveSemantics&& rhs);

private:
	Texture* m_pTexture{};
	std::string m_SomeTextForMoving{};
};

