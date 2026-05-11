#include "pch.h"
#include "Game.h"

#include "utils.h"
#include "Texture.h"

#include <iostream>


Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_pWall = new Texture{ "office_background.png" };
}

void Game::Cleanup( )
{
	delete m_pWall;
}

void Game::Update( float elapsedSec )
{
	std::cout << 1 / elapsedSec << std::endl;
	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
}

void Game::Draw( ) const
{
	ClearBackground( );


	m_pWall->Draw(Vector2f{}, GetViewPort());


	if (drawBlendCircle)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		drawGlowCircle(100.f, 200.f, 200.f, 10);

		glDisable(GL_BLEND);
	}
	else if (drawBaseCircle)
	{
		drawGlowCircle(100.f, 200.f, 200.f, 10);
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	switch ( e.keysym.sym )
	{
	case SDLK_SPACE:
		drawBlendCircle = true;
		break;
	case SDLK_1:
		drawBaseCircle = true;
		break;
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch (e.keysym.sym)
	{
	case SDLK_SPACE:
		drawBlendCircle = false;
		break;
	case SDLK_1:
		drawBaseCircle = false;
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}


void Game::drawGlowCircle(float cx, float cy, float radius, int segments) const {
	glBegin(GL_TRIANGLE_FAN);
	// The Center: This is where the light is strongest
	// Color: Purple (0.5, 0.0, 0.5) with 50% transparency (0.5)
	glColor4f(1.f, 1.f, 1.f, 0.3f);
	glVertex2f(cx, cy);


	// The Edges: These fade out to nothing
	glColor4f(1.f, 1.f, 1.f, 0.0f);
	for (int i = 0; i <= segments; i++) {
		float angle = i * 2.0f * 3.14159f / segments;
		float x = cos(angle) * radius;
		float y = sin(angle) * radius;
		glVertex2f(cx + x, cy + y);
	}
	glEnd();
}