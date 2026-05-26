#pragma once
#include <vector>
#include <fstream>
#include <iostream>

#include "EnvironmentActiveObject.h"
#include "EnvironmentCosmeticObject.h"
#include "AnimationFrameInfo.h"
#include "ControlPoint.h"
#include "SpriteManager.h"
#include "Enemy.h"

class InteractableObject;

using Json = nlohmann::json;
void from_json(const Json& j, Rectf& rect);
void from_json(const Json& j, AnimationFrameInfo& anFrame);

class JsonImporter
{
public:
	struct GameData {
		std::vector<EnvironmentActiveObject> activeObjects;
		std::vector<EnvironmentCosmeticObject> cosmeticObjects;
		std::vector<InteractableObject*> interactableObjects;
		std::vector<ControlPoint> controlPoints;

		struct EnemyInfo {
			Enemy::EnemyType type{};
			Vector2f position{};
			float speed{};
			float scale{};
			int floor{};
		};

		std::vector<EnemyInfo> enemiesInfo;

		Vector2f respawnPoint{};
		float playerSpeed{};
		float playerScale{};
		int playerFloor{};
	};


	static GameData ImportGameInfo(const std::string& jsonPath, SpriteManager& spriteManager) ;
	static std::vector<AnimationFrameInfo> ImportAnimationFrameObjects(const std::string& jsonPath) ;
private:
	static Json ParseJsonFile(const std::string& jsonPath) ;
	static void ProcessJsonObject(const Json& object, GameData& dst, SpriteManager& spriteManager) ;

	static void AddPlayerInfo(const Json& object, GameData& dst) ;
	static void AddEnemy(Enemy::EnemyType type, const Json& object, GameData& dst) ;
	static void AddControlPoint(const Json& object, GameData& dst) ;
	static void AddCosmeticObject(const Json& object, GameData& dst, SpriteManager& spriteManager) ;
	static void AddActiveObject(const Json& object, GameData& dst, SpriteManager& spriteManager) ;
	static void AddInteractableObject(const Json& object, GameData& dst, SpriteManager& spriteManager);

	static EnvironmentActiveObject::EnvironmentObjectType StringToObjectType(const std::string& typeStr);
	static ControlPoint::ControlPointType				   StringToControlPointType(const std::string& typeStr) ;

};

