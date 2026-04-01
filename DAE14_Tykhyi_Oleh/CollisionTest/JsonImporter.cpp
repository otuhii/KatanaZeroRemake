#include "pch.h"
#include "JsonImporter.h"

void from_json(const Json& j, Rectf& rect)
{
	rect.left = j.at("left").get<float>();
	rect.bottom = j.at("bottom").get<float>();
	rect.width = j.at("width").get<float>();
	rect.height = j.at("height").get<float>();
}

//TODO add Rectf support for this class
void from_json(const Json& j, EnvironmentObject& envObj)
{
	envObj.texturePath = j.value("texturePath", "default.png");
	envObj.position.x = j.at("xPosition").get<float>();
	envObj.position.y = j.at("yPosition").get<float>();
	envObj.objectName = j.value("objectName", "Unknown");

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


std::vector<EnvironmentObject> JsonImporter::ImportEnvironmentObjects(const std::string& jsonPath) const
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
			return data["objects"][0]["prog2GameObjects"].get<std::vector<EnvironmentObject>>();
		}
	}
	catch (const Json::exception& exception)
	{
		std::cout << "JSON parsing Error: " << exception.what() << std::endl 
			<< "PATH-> " << jsonPath << std::endl;
	}

	return {};
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
