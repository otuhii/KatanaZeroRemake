#pragma once
#include "BaseGame.h"

#include "JsonImporter.h"

class SpriteManager;
class EnemyManager;
class CollisionManager;

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
	void CreateEnemies			(const JsonImporter::GameData& gameData);
	void MapSetup				(const JsonImporter::GameData& gameData);

	JsonImporter		m_JsonImporter{}; //TODO maybe change this to pointer later

	EnemyManager*		m_pEnemyManager{};
	SpriteManager*		m_pSpriteManager{};
	CollisionManager*	m_pCollisionManager{};
	
	Map*				m_pMap;
	Player*				m_pPlayer{};
};