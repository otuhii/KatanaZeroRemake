#include "pch.h"
#include "Game.h"

#include "CollisionManager.h"
#include "SpriteManager.h"
#include "EnemyManager.h"

#include "Player.h"
#include "Map.h"

#include <iostream>
#include "utils.h"

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
	JsonImporter::GameData
		importedGameInfo{};

	m_pSpriteManager = new SpriteManager();
	m_pCollisionManager = new CollisionManager();
	m_pEnemyManager = new EnemyManager{};
	m_pMap = new Map{};

	importedGameInfo = m_JsonImporter.ImportGameInfo("json/GameInfo.json", *m_pSpriteManager);

	m_pPlayer = new Player(
		m_pSpriteManager->CreateSprite("img/chr/zero_spritesheet.png"),
		m_pSpriteManager->CreateSprite("img/chr/SplashAnimation.png"),
		m_JsonImporter.ImportAnimationFrameObjects("json/PlayerAnimationFramesInfo.json"),
		importedGameInfo.respawnPoint,
		importedGameInfo.playerSpeed,
		importedGameInfo.playerScale,
		importedGameInfo.playerFloor
	);
	
	MapSetup(importedGameInfo);
	EnemyTypeInitialization();
	CreateEnemies(importedGameInfo);
}

void Game::Cleanup( )
{
	delete m_pMap;
	delete m_pSpriteManager;
	delete m_pCollisionManager;
	delete m_pPlayer;
	delete m_pEnemyManager;
}

void Game::Update( float elapsedSec )
{
	//std::cout << 1 / elapsedSec << std::endl;

	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	
	m_pPlayer->Update(elapsedSec, pStates, GetViewPort());

	m_pCollisionManager->HandleMovement(m_pPlayer, *m_pMap,  elapsedSec, true);

	m_pEnemyManager->Update( elapsedSec, m_pPlayer->GetPosition(), m_pPlayer->GetFloor(), m_pMap, m_pCollisionManager);

	m_pSpriteManager->Update(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );

	m_pMap->Draw();

	m_pPlayer->Draw();

	m_pEnemyManager->Draw();
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

void Game::EnemyTypeInitialization()
{
	m_pEnemyManager->InitializeEnemyType(
		Enemy::EnemyType::grunt,
		m_pSpriteManager->CreateSprite("img/chr/grunt_spritesheet.png"),
		m_JsonImporter.ImportAnimationFrameObjects("json/enemy_Grunt_FrameInfo.json")
	);

	m_pEnemyManager->InitializeEnemyType(
		Enemy::EnemyType::gangster,
		m_pSpriteManager->CreateSprite("img/chr/gangster_spritesheet.png"),
		m_JsonImporter.ImportAnimationFrameObjects("json/enemy_Gangster_FrameInfo.json")
	);
}

void Game::CreateEnemies(const JsonImporter::GameData& gameData)
{
	for (const JsonImporter::GameData::EnemyInfo& enemyInfo : gameData.enemiesInfo)
	{
		m_pEnemyManager->AddEnemy(
			enemyInfo.type,
			enemyInfo.position,
			enemyInfo.speed,
			enemyInfo.scale,
			enemyInfo.floor
		);
	}
}

void Game::MapSetup(const JsonImporter::GameData& gameData)
{
	m_pMap->SetEnvironmentActiveObjects(gameData.activeObjects);
	m_pMap->SetEnvironmentCosmeticObjects(gameData.cosmeticObjects);
	m_pMap->SetControlPoints(gameData.controlPoints);
}



