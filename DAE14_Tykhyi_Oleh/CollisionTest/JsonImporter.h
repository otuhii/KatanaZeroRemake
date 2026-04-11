#pragma once
#include <vector>
#include <fstream>
#include <iostream>

#include "EnvironmentActiveObject.h"
#include "EnvironmentCosmeticObject.h"
#include "AnimationFrameInfo.h"
#include "SpriteManager.h"
#include "Enemy.h"

using Json = nlohmann::json;
void from_json(const Json& j, Rectf& rect);
void from_json(const Json& j, AnimationFrameInfo& anFrame);

class JsonImporter
{
public:
	struct GameData {
		std::vector<EnvironmentActiveObject> activeObjects;
		std::vector<EnvironmentCosmeticObject> cosmeticObjects;
		std::vector<Vector2f> controlPoints;

		struct EnemyInfo {
			Enemy::EnemyType type{};
			Vector2f position{};
			float speed{};
			float scale{};
		};

		std::vector<EnemyInfo> enemiesInfo;

		Vector2f respawnPoint{};
		float playerSpeed{};
		float playerScale{};
	};


	GameData ImportGameInfo(const std::string& jsonPath, SpriteManager& spriteManager) const;
	std::vector<AnimationFrameInfo> ImportAnimationFrameObjects(const std::string& jsonPath) const;
private:
	Json ParseJsonFile(const std::string& jsonPath) const;
	void ProcessJsonObject(const Json& object, GameData& dst, SpriteManager& spriteManager) const;

	void AddPlayerInfo(const Json& object, GameData& dst) const;
	void AddEnemy(Enemy::EnemyType type, const Json& object, GameData& dst) const;
	void AddControlPoint(const Json& object, GameData& dst) const;
	void AddCosmeticObject(const Json& object, GameData& dst, SpriteManager& spriteManager) const;
	void AddActiveObject(const Json& object, GameData& dst, SpriteManager& spriteManager) const;

	EnvironmentActiveObject::EnvironmentObjectType StringToType(const std::string& typeStr) const;
};

