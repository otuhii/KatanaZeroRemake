#include "pch.h"
#include "Game.h"

#include "CollisionManager.h"
#include "SpriteManager.h"
#include "Player.h"
#include "Map.h"

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
	m_pSpriteManager = new SpriteManager();

	m_pCollisionManager = new CollisionManager();

	m_pMap = new Map{};

	m_JsonImporter.ImportEnvironmentObjects("json/GameEnvironment.json", *m_pMap, *m_pSpriteManager);

	m_pPlayer = new Player(
		m_pSpriteManager->CreateSprite("img/ProcessedPlayerSpriteSheet.png"),
		m_pSpriteManager->CreateSprite("img/SplashAnimation.png"),
		m_JsonImporter.ImportAnimationFrameObjects("json/PlayerAnimationFramesInfo.json"),
		Vector2f{ 100.f, 100.f }, 
		250.f);
}

void Game::Cleanup( )
{
	delete m_pMap;
	delete m_pSpriteManager;
	delete m_pCollisionManager;
	delete m_pPlayer;
}

void Game::Update( float elapsedSec )
{
	std::cout << 1 / elapsedSec << std::endl;

	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	
	m_pPlayer->Update(elapsedSec, pStates, GetViewPort());

	m_pCollisionManager->HandleMovement(m_pPlayer, *m_pMap, elapsedSec);

	m_pSpriteManager->Update(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );

	m_pMap->Draw();

	m_pPlayer->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	m_pPlayer->ProcessMouseUpEvent(e, GetViewPort());
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

