#pragma once
#include <vector>
#include <iostream>
#include <fstream>


struct GameObject {
	Vector2f position;
	std::string texturePath;
	std::string objectName;
};

using Json = nlohmann::json;
void from_json(const Json& j, GameObject& g);

class JsonImporter
{
public:
	std::vector<GameObject> ImportGameObjects(const std::string& jsonPath);

private:
	Json ParseJsonFile(const std::string& jsonPath);
};

