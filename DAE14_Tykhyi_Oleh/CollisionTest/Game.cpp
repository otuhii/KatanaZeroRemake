#include "pch.h"
#include "Game.h"

#include "CollisionManager.h"
#include "SpriteManager.h"
#include "EnemyManager.h"
#include "ParticleManager.h"
#include "CombatManager.h"
#include "SoundManager.h"

#include "Camera.h"
#include "Cursor.h"
#include "Hud.h"

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
	m_pSpriteManager = new SpriteManager{};
	m_pCollisionManager = new CollisionManager{};
	m_pMap = new Map{};
	m_pParticleManager = new ParticleManager{100, 300, m_pSpriteManager};
	m_pCombatManager = new CombatManager{};
	m_pSoundManager = new SoundManager{};


	m_pCamera = new Camera{ GetViewPort().width, GetViewPort().height };
	m_pCursor = new Cursor{ m_pSpriteManager->CreateSprite("img/spr_cursor.png") };

	JsonImporter::GameData
		importedGameInfo{};
	importedGameInfo = JsonImporter::ImportGameInfo("json/GameInfo.json", *m_pSpriteManager);

	m_pPlayer = new Player(
		m_pSpriteManager->CreateSprite("img/chr/zero_spritesheet.png"),
		m_pSpriteManager->CreateSprite("img/chr/SplashAnimation.png"),
		m_pSpriteManager->CreateSprite("img/chr/spr_swordprojectile.png"),
		JsonImporter::ImportAnimationFrameObjects("json/PlayerAnimationFramesInfo.json"),
		JsonImporter::ImportAnimationFrameObjects("json/PlayerRangeProjectileAnimatFrameInfo.json"),
		importedGameInfo.respawnPoint,
		importedGameInfo.playerSpeed,
		importedGameInfo.playerScale,
		importedGameInfo.playerFloor
	);
	m_pEnemyManager = new EnemyManager{m_pPlayer, m_pSoundManager};

	m_pHud = new Hud{ GetViewPort(), m_pPlayer, m_pSpriteManager };


	MapSetup(importedGameInfo);
	EnemyTypeInitialization(importedGameInfo);
	CreateEnemies(importedGameInfo, m_pSpriteManager);
}

void Game::Cleanup( )
{
	delete m_pMap;
	delete m_pSpriteManager;
	delete m_pCollisionManager;
	delete m_pPlayer;
	delete m_pEnemyManager;
	delete m_pParticleManager;
	delete m_pCombatManager;
	delete m_pSoundManager;
	delete m_pCamera;
	delete m_pCursor;
	delete m_pHud;
}

void Game::Update( float elapsedSec )
{
	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );

	FPS(elapsedSec);

	float
		timeDivider{ 1.f };

	if (m_SlowMo)
	{
		timeDivider = 0.2f;
	}


	m_pHud->Update(elapsedSec, pStates);

	m_pPlayer->Update(timeDivider*elapsedSec, m_pMap, pStates, GetViewPort(), m_pParticleManager, m_pSoundManager);

	m_pCollisionManager->HandleMovement(m_pPlayer, m_pMap, timeDivider*elapsedSec, true);

	m_pEnemyManager->Update(timeDivider*elapsedSec, m_pMap, m_pParticleManager, m_pCollisionManager);

	m_pCombatManager->ResolveCombat(m_pPlayer, m_pEnemyManager, m_pParticleManager, m_pMap, m_pSoundManager);

	m_pSpriteManager->Update(timeDivider*elapsedSec);

	m_pParticleManager->Update(timeDivider * elapsedSec);

	m_pCollisionManager->HandleParticles(m_pParticleManager, m_pSoundManager, m_pMap);

	m_pMap->Update(timeDivider * elapsedSec, m_pSoundManager);

	m_pCamera->Update(timeDivider * elapsedSec, m_pPlayer->GetPosition(), 1756.f, 750.f);
}

void Game::Draw( ) const
{
	ClearBackground( );
	PaintBlackBackground();

	m_pCamera->Aim();

	m_pMap->Draw();

	m_pPlayer->Draw();

	m_pEnemyManager->Draw();

	m_pParticleManager->Draw();

	m_pCamera->Reset();

	m_pCursor->Draw();

	m_pHud->Draw();

}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	switch (e.keysym.sym)
	{
	case SDLK_LSHIFT:
		m_SlowMo = true;
		break;
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch ( e.keysym.sym )
	{
	case SDLK_LSHIFT:
		m_SlowMo = false;
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	m_pCursor->Update(e);
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	m_pPlayer->ProcessMouseUpEvent(e, m_pCamera->GetBasePoint(), m_pParticleManager, m_pSoundManager, GetViewPort());
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::EnemyTypeInitialization(const JsonImporter::GameData& gameData)
{
	const float
		gruntAttackRange{ 50.f },
		gruntDetectionRange{ 400.f };

	const float
		gangsterAttackRange{ 700.f },
		gangsterDetectionRange{ 500.f };

	m_pEnemyManager->SetControlPoints(gameData.controlPoints);

	m_pEnemyManager->InitializeEnemyType(
		Enemy::EnemyType::grunt,
		m_pSpriteManager->CreateSprite("img/chr/grunt_spritesheet.png"),
		m_pSpriteManager->CreateSprite("img/chr/spr_grunt_slash.png"),
		nullptr,
		JsonImporter::ImportAnimationFrameObjects("json/enemy_Grunt_FrameInfo.json"),
		JsonImporter::ImportAnimationFrameObjects("json/GruntSlashAnimationFramesInfo.json"),
		gruntDetectionRange,
		gruntAttackRange
	);

	m_pEnemyManager->InitializeEnemyType(
		Enemy::EnemyType::gangster,
		m_pSpriteManager->CreateSprite("img/chr/gangster_spritesheet.png"),
		m_pSpriteManager->CreateSprite("img/chr/gangster_gun_sprite.png"),
		m_pSpriteManager->CreateSprite("img/chr/gangster_bulletprojectile_sprite.png"),
		JsonImporter::ImportAnimationFrameObjects("json/enemy_Gangster_FrameInfo.json"),
		std::vector<AnimationFrameInfo>{},
		gangsterDetectionRange,
		gangsterAttackRange
	);
}

void Game::CreateEnemies(const JsonImporter::GameData& gameData, SpriteManager* pSpriteManager)
{
	for (const JsonImporter::GameData::EnemyInfo& enemyInfo : gameData.enemiesInfo)
	{
		m_pEnemyManager->AddEnemy(
			enemyInfo.type,
			enemyInfo.position,
			enemyInfo.speed,
			enemyInfo.scale,
			enemyInfo.floor,
			pSpriteManager
		);
	}
}

void Game::MapSetup(const JsonImporter::GameData& gameData)
{
	m_pMap->SetEnvironmentActiveObjects(gameData.activeObjects);
	m_pMap->SetEnvironmentCosmeticObjects(gameData.cosmeticObjects);
	m_pMap->SetInteractableObjects(gameData.interactableObjects);
}

void Game::PaintBlackBackground() const
{
	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
	utils::FillRect(GetViewPort());
}

void Game::FPS(float elapsedSec)
{
	m_FPSAccumulatedTime += elapsedSec;
	m_FPSFrames++;

	if (m_FPSAccumulatedTime >= 1.0f) 
	{
		m_CurrentAverageFPS = m_FPSFrames;
		m_FPSFrames = 0;
		m_FPSAccumulatedTime -= 1.0f;

		system("CLS");
		std::cout << "FPS: " << m_CurrentAverageFPS << std::endl;
	}
}



