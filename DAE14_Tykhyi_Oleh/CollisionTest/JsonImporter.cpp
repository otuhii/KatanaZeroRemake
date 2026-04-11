#include "pch.h"
#include "JsonImporter.h"

void from_json(const Json& j, Rectf& rect)
{
	rect.left = j.at("left").get<float>();
	rect.bottom = j.at("bottom").get<float>();
	rect.width = j.at("width").get<float>();
	rect.height = j.at("height").get<float>();
}

void from_json(const Json& j, AnimationFrameInfo& anFrame)
{
	anFrame.frameTime = j.at("frameTime").get<float>();
	anFrame.maxFrameCount = j.at("maxFrameCount").get<int>();

	if (j.contains("dimensionRect") && !j["dimensionRect"].is_null())
	{
		anFrame.frameDimension = j.at("dimensionRect").get<Rectf>();
	}
}



JsonImporter::GameData JsonImporter::ImportGameInfo(const std::string& jsonPath, SpriteManager& spriteManager) const
{
	Json data{ ParseJsonFile(jsonPath) };

	if (data.is_discarded() || data.empty()) return GameData{};

	GameData processedData{};

	try
	{
		if (data.contains("objects") && !data["objects"].empty())
		{
			Json envObjects = data["objects"][0]["prog2GameObjects"];

			for (size_t index{ 0 }; index < envObjects.size(); ++index)
			{
				Json obj = envObjects[index];

				ProcessJsonObject(obj, processedData, spriteManager);
			}
		}
	}
	catch (const nlohmann::json::exception& exception)
	{
		std::cout << "JSON parsing Error: " << exception.what() << std::endl
			<< "PATH-> " << jsonPath << std::endl;
	}


	return processedData;
}

std::vector<AnimationFrameInfo> JsonImporter::ImportAnimationFrameObjects(const std::string& jsonPath) const
{
	Json data{ ParseJsonFile(jsonPath) };

	if (data.is_discarded() ||
		data.empty())
	{
		return {};
	}

	try
	{
		if (data.contains("frames") && data["frames"].is_array() && !data["frames"].empty())
		{
			return data["frames"].get<std::vector<AnimationFrameInfo>>();
		}
	}

	catch (const Json::exception & exception)
	{
		std::cout << "JSON parsing Error: " << exception.what() << std::endl
			<< "PATH-> " << jsonPath << std::endl;
	}

	return {};
}

Json JsonImporter::ParseJsonFile(const std::string& jsonPath) const
{
	std::ifstream inputFile{ jsonPath };
	if (!inputFile.is_open())
	{
		std::cout << "Count not open the json file: " << jsonPath << std::endl;
		return Json::value_t::discarded;
	}

	Json parsedData;

	try {
		inputFile >> parsedData;
	}
	catch (const Json::parse_error& exception)
	{
		std::cout << "Parse error: " << exception.what() << std::endl;
		return Json::value_t::discarded;
	}

	inputFile.close();

	return parsedData;
}

void JsonImporter::ProcessJsonObject(const Json& object, GameData& dst, SpriteManager& spriteManager) const
{
	std::string
		objectType{ object.at("objectType").get<std::string>() };

	if (objectType == "playerinfo")
	{
		AddPlayerInfo(object, dst);
	}
	else if (objectType == "enemygrunt")
	{
		AddEnemy(Enemy::EnemyType::grunt, object, dst);
	}
	else if (objectType == "enemygangster")
	{
		AddEnemy(Enemy::EnemyType::gangster, object, dst);
	}
	else if (objectType == "controlpoint")
	{
		AddControlPoint(object, dst);
	}
	else if (objectType == "platform" || objectType == "stairs" || objectType == "jumpthroughplatform")
	{
		AddActiveObject(object, dst, spriteManager);
	}
	else if (objectType == "cosmetic")
	{
		AddCosmeticObject(object, dst, spriteManager);
	}
}

void JsonImporter::AddPlayerInfo(const Json& object, GameData& dst) const
{
	dst.playerSpeed = object.at("speed").get<float>();
	dst.respawnPoint.x = object.at("xPosition").get<float>();
	dst.respawnPoint.y = object.at("yPosition").get<float>();
	dst.playerScale = object.at("scale").get<float>();
}

void JsonImporter::AddEnemy(Enemy::EnemyType type, const Json& object, GameData& dst) const
{
	Vector2f position{
		object.at("xPosition").get<float>(),
		object.at("yPosition").get<float>()
	};

	float
		speed{ object.at("speed").get<float>() };

	float
		scale{ object.at("scale").get<float>() };

	dst.enemiesInfo.push_back(GameData::EnemyInfo{
		type,
		position,
		speed,
		scale
	});
}

void JsonImporter::AddControlPoint(const Json& object, GameData& dst) const
{
	dst.controlPoints.push_back(Vector2f{
		object.at("xPosition").get<float>(),
		object.at("yPosition").get<float>()
	});
}

void JsonImporter::AddCosmeticObject(const Json& object, GameData& dst, SpriteManager& spriteManager ) const
{
	std::string
		texPath{ object.value("texturePath", "default.png") };

	Sprite*
		pTexture{ nullptr };

	if (texPath != "default.png")
	{
		pTexture = spriteManager.CreateSprite("img/env/" + texPath);
		pTexture->SetScale(object.at("scale").get<float>());
	}

	dst.cosmeticObjects.push_back(EnvironmentCosmeticObject{
		object.at("xPosition").get<float>(),
		object.at("yPosition").get<float>(),
		pTexture
	});
}

void JsonImporter::AddActiveObject(const Json& object, GameData& dst, SpriteManager& spriteManager) const
{
	if (object.contains("colliders") && object["colliders"].is_array())
	{
		std::string
			texPath{ object.value("texturePath", "default.png") };

		std::vector<Rectf>
			colliders{object.at("colliders").get<std::vector<Rectf>>()};

		Sprite*
			pTexture{ nullptr };

		if (texPath != "default.png")
		{
			pTexture = spriteManager.CreateSprite("img/env/" + texPath);
			pTexture->SetScale(object.at("scale").get<float>());
		}

		dst.activeObjects.push_back(EnvironmentActiveObject{
			object.at("xPosition").get<float>(),
			object.at("yPosition").get<float>(),
			colliders,
			pTexture,
			StringToType(object.at("objectType").get<std::string>())
		});
	}
}

EnvironmentActiveObject::EnvironmentObjectType JsonImporter::StringToType(const std::string& typeStr) const
{
	if (typeStr == "stairs")
	{
		return EnvironmentActiveObject::EnvironmentObjectType::stairs;
	}
	if (typeStr == "jumpthroughplatform")
	{
		return EnvironmentActiveObject::EnvironmentObjectType::jumpThroughPlatform;
	}
	return EnvironmentActiveObject::EnvironmentObjectType::platform;
}