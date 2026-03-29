#include "pch.h"
#include "JsonImporter.h"

void from_json(const Json& j, GameObject& g)
{
	g.texturePath = j.value("texturePath", "default.png");
	g.position.x = j.at("xPosition").get<float>();
	g.position.y = j.at("yPosition").get<float>();
	g.objectName = j.value("objectName", "Unknown");
}


std::vector<GameObject> JsonImporter::ImportGameObjects(const std::string& jsonPath)
{
	Json data{ ParseJsonFile(jsonPath) };

	if (data.is_discarded() ||
		data.empty())
	{
		return {};
	}

	try 
	{
		if (data.contains("objects") && data["objects"].is_array() && !data["objects"].empty())
		{
			return data["objects"][0]["prog2GameObjects"].get<std::vector<GameObject>>();
		}
	}
	catch (const Json::exception& exception)
	{
		std::cout << "JSON parsing Error: " << exception.what() << std::endl 
			<< "PATH-> " << jsonPath << std::endl;
	}

	return {};
}



Json JsonImporter::ParseJsonFile(const std::string& jsonPath)
{
	std::ifstream inputFile{ jsonPath };
	if (!inputFile.is_open())
	{
		std::cout << "Count not open the json file" << std::endl;
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
