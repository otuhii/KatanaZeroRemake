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

EnvironmentActiveObject::EnvironmentObjectType StringToType(const std::string& typeStr)
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

void JsonImporter::ImportEnvironmentInfo(const std::string& jsonPath, Map& gameMap, SpriteManager& SpriteManager, EnemyManager& enemyManager, float& playerSpeed) const
{
	Json data{ ParseJsonFile(jsonPath) };

	std::vector<EnvironmentActiveObject> activeObjects{};
	std::vector<EnvironmentCosmeticObject> cosmeticObjects{};


	if (data.is_discarded() ||
		data.empty())
	{
		return;
	}

	try 
	{
		if (data.contains("objects") && !data["objects"].empty())
		{
			Json envObjects = data["objects"][0]["prog2GameObjects"];

			for (size_t index{ 0 }; index < envObjects.size(); ++index)
			{
				Json obj = envObjects[index];

				float
					x{ obj.at("xPosition").get<float>() },
					y{ obj.at("yPosition").get<float>() };

				std::string objectType{ obj.at("objectType").get<std::string>() };

				if (objectType == "playerinfo")
				{
					gameMap.SetRespawnPoint(Vector2f{ x, y });
					playerSpeed = obj.at("speed").get<float>();
				}
				else if (objectType == "enemygrunt")
				{
					enemyManager.AddEnemy(
						Enemy::EnemyType::grunt,
						Vector2f{ x, y },
						obj.at("speed").get<float>(),
						2.f
					);
				}
				else if (objectType == "enemygangster")
				{
					enemyManager.AddEnemy(
						Enemy::EnemyType::gangster,
						Vector2f{ x,y },
						obj.at("speed").get<float>(),
						2.f
					);
				}
				else
				{
					std::string
						texPath{ obj.value("texturePath", "default.png") };

					Sprite* pTexture{ nullptr };
					if (texPath != "default.png")
					{
						pTexture = SpriteManager.CreateSprite("img/" + texPath);
						pTexture->SetScale(obj.at("scale").get<float>());
					}

					std::vector<Rectf> colliders;

					if (obj.contains("colliders") && obj["colliders"].is_array())
					{
						colliders = obj.at("colliders").get<std::vector<Rectf>>();

						activeObjects.push_back(EnvironmentActiveObject
							{
								x,
								y,
								colliders,
								pTexture,
								StringToType(objectType)
							});
					}
					else
					{
						cosmeticObjects.push_back(EnvironmentCosmeticObject
							{
								x,
								y,
								pTexture
							});
					}
				}
			}
		}
	}
	catch (const nlohmann::json::exception& exception)
	{
		std::cout << "JSON parsing Error: " << exception.what() << std::endl
			<< "PATH-> " << jsonPath << std::endl;
	}

	gameMap.SetEnvironmentActiveObjects(activeObjects);
	gameMap.SetEnvironmentCosmeticObjects(cosmeticObjects);
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
