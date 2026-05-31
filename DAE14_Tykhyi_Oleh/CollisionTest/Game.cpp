#include "pch.h"
#include "Game.h"

#include "CollisionManager.h"
#include "SpriteManager.h"
#include "EnemyManager.h"
#include "ParticleManager.h"
#include "CombatManager.h"
#include "SoundManager.h"
#include "LevelManager.h"

#include "ScreenOverlay.h"

#include "Camera.h"
#include "Cursor.h"
#include "Hud.h"

#include "Player.h"
#include "Map.h"

#include "utils.h"
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
	m_pSpriteManager = new SpriteManager{};
	m_pMap = new Map{};
	m_pParticleManager = new ParticleManager{100, 300, m_pSpriteManager};
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

	EnemyTypeInitialization(importedGameInfo);
	CreateEnemies(importedGameInfo, m_pSpriteManager);
	MapSetup(importedGameInfo);

	m_pLevelManager = new LevelManager{m_pPlayer, m_pEnemyManager};
	m_pScreenOverlay = new ScreenOverlay{ m_pSpriteManager, GetViewPort(), m_pLevelManager };

	m_pLevelManager->LinkParticleManager(m_pParticleManager);
	m_pLevelManager->LinkSoundManager(m_pSoundManager);
	m_pLevelManager->LinkMap(m_pMap);
	m_pParticleManager->LinkLevelManager(m_pLevelManager);
	m_pSoundManager->LinkLevelManager(m_pLevelManager);
}

void Game::Cleanup( )
{
	delete m_pMap;
	delete m_pSpriteManager;
	delete m_pPlayer;
	delete m_pEnemyManager;
	delete m_pParticleManager;
	delete m_pSoundManager;
	delete m_pCamera;
	delete m_pCursor;
	delete m_pLevelManager;
	delete m_pScreenOverlay;
}

void Game::Update( float elapsedSec )
{
	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );

	FPS(elapsedSec);
	float
		timeDivider{ 1.f };

	timeDivider = m_pLevelManager->GetTimeMultiplier();

	m_pSpriteManager->Update(timeDivider * elapsedSec);
	m_pParticleManager->Update(timeDivider * elapsedSec);
	m_pLevelManager->Update(elapsedSec, pStates);

	m_pScreenOverlay->Update(elapsedSec, pStates);

	if (m_pLevelManager->GetCurrentState() == LevelManager::LevelState::Gameplay)
	{
		m_pPlayer->Update(timeDivider * elapsedSec, m_pMap, pStates, GetViewPort(), m_pParticleManager, m_pSoundManager);

		CollisionManager::HandleMovement(m_pPlayer, m_pMap, timeDivider * elapsedSec, true);

		m_pEnemyManager->Update(timeDivider * elapsedSec, m_pMap, m_pParticleManager);

		CollisionManager::HandleParticles(m_pParticleManager, m_pSoundManager, m_pMap);

		CombatManager::ResolveCombat(m_pPlayer, m_pEnemyManager, m_pParticleManager, m_pMap, m_pSoundManager);

		m_pMap->Update(timeDivider * elapsedSec, m_pSoundManager, m_pPlayer);

		m_pCamera->Update(elapsedSec, m_pCursor->GetPosition(), m_pPlayer->GetPosition(), 1756.f, 750.f);
	}
	else
	{
		m_pCamera->Update(elapsedSec, Vector2f{}, m_pPlayer->GetPosition(), 1756.f, 750.f);
	}
	
}

void Game::Draw( ) const
{
	ClearBackground( );

	m_pCamera->Aim();

	m_pMap->Draw();

	m_pPlayer->Draw();

	m_pEnemyManager->Draw();

	m_pParticleManager->Draw();

	m_pCamera->Reset();

	if (m_pLevelManager->GetCurrentState() == LevelManager::LevelState::Gameplay)
	{
		m_pCursor->Draw();
	}

	m_pScreenOverlay->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	/*switch (e.keysym.sym)
	{
	case SDLK_LEFT:
		m_pLevelManager->Backward();
		break;
	case SDLK_RIGHT:
		m_pLevelManager->Forward();
		break;
	}*/
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
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
	if (m_pLevelManager->GetCurrentState() == LevelManager::LevelState::Gameplay)
	{
		m_pPlayer->ProcessMouseUpEvent(e, m_pCamera->GetBasePoint(), m_pParticleManager, m_pSoundManager, GetViewPort());
		m_pLevelManager->ProcessMouseUpEvent(e);
	}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
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



