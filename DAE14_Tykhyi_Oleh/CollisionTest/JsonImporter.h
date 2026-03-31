#pragma once
#include <vector>
#include <fstream>
#include <iostream>

#include "EnvironmentObject.h"

using Json = nlohmann::json;
void from_json(const Json& j, EnvironmentObject& g);

class JsonImporter
{
public:
	std::vector<EnvironmentObject> ImportEnvironmentObjects(const std::string& jsonPath);

private:
	Json ParseJsonFile(const std::string& jsonPath);
};

