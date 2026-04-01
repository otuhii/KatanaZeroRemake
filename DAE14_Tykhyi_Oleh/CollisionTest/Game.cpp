#include "pch.h"
#include "Game.h"
#include "SpriteManager.h"
#include "Player.h"

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

	m_pPlayer = new Player(
		m_pSpriteManager->CreateSprite("img/ProcessedPlayerSpriteSheet.png"),
		m_pSpriteManager->CreateSprite("img/SplashAnimation.png"),
		m_JsonImporter.ImportAnimationFrameObjects("json/PlayerAnimationFramesInfo.json"),
		Vector2f{ 30.f, 30.f }, 
		300.f);
}

void Game::Cleanup( )
{
	delete m_pSpriteManager;
	delete m_pPlayer;
}

void Game::Update( float elapsedSec )
{
	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	
	m_pPlayer->Update(elapsedSec, pStates, GetViewPort());

	m_pSpriteManager->Update(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );

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
	m_pPlayer->ProcessMouseUpEvent(e);
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}


void Game::TestJsonExporter()
{
	std::vector<EnvironmentObject> environmentObjects{ m_JsonImporter.ImportEnvironmentObjects("json/Prog2UnityExporterData.json") };

	if (environmentObjects.size() > 0)
	{
		for (const EnvironmentObject& environmentObject : environmentObjects)
		{
			std::cout << environmentObject.position << std::endl;
			std::cout << environmentObject.objectName << std::endl;
			std::cout << environmentObject.texturePath << std::endl;
			std::cout << "===========================================" << std::endl;
		}

	}


	std::vector<AnimationFrameInfo> animationFrames{ m_JsonImporter.ImportAnimationFrameObjects("json/PlayerAnimationFramesInfo.json") };

	if (animationFrames.size() > 0)
	{
		for (const AnimationFrameInfo& animationFrame : animationFrames)
		{
			std::cout << Vector2f{ animationFrame.frameDimension.width, animationFrame.frameDimension.height } << std::endl;
			std::cout << animationFrame.frameTime << std::endl;
			std::cout << animationFrame.maxFrameCount << std::endl;
			std::cout << "==============================================" << std::endl;
		}
	}
}