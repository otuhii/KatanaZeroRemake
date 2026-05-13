#pragma once
#include "BaseGame.h"
#include "JsonImporter.h"

class SpriteManager;
class EnemyManager;
class CollisionManager;
class ParticleManager;
class CombatManager;
class SoundManager;
class LevelManager;

class ScreenOverlay;

class Camera;
class Cursor;
class Hud;

class Player;
class Map;

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void EnemyTypeInitialization(const JsonImporter::GameData& gameData);
	void CreateEnemies			(const JsonImporter::GameData& gameData, SpriteManager* pSpriteManager);
	void MapSetup				(const JsonImporter::GameData& gameData);

#pragma region FPS
	float
		m_FPSAccumulatedTime{ 0.f };
	int
		m_FPSFrames{ 0 },
		m_CurrentAverageFPS{ 0 };
	void FPS(float elapsedSec);
#pragma endregion FPS

	ScreenOverlay*		m_pScreenOverlay{};
	Camera*				m_pCamera{};
	Cursor*				m_pCursor{};
	Hud*				m_pHud{};

	CombatManager*		m_pCombatManager{};
	EnemyManager*		m_pEnemyManager{};
	SpriteManager*		m_pSpriteManager{};
	CollisionManager*	m_pCollisionManager{};
	ParticleManager*	m_pParticleManager{};
	SoundManager*		m_pSoundManager{};
	LevelManager*		m_pLevelManager;

	Map*				m_pMap;
	Player*				m_pPlayer{};
};