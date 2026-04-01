#pragma once
#include <vector>
#include <fstream>
#include <iostream>

#include "EnvironmentObject.h"
#include "AnimationFrameInfo.h"

using Json = nlohmann::json;
void from_json(const Json& j, Rectf& rect);
void from_json(const Json& j, EnvironmentObject& envObj);
void from_json(const Json& j, AnimationFrameInfo& anFrame);

class JsonImporter
{
public:
	std::vector<EnvironmentObject> ImportEnvironmentObjects(const std::string& jsonPath) const;
	std::vector<AnimationFrameInfo> ImportAnimationFrameObjects(const std::string& jsonPath) const;
private:
	Json ParseJsonFile(const std::string& jsonPath) const;
};

