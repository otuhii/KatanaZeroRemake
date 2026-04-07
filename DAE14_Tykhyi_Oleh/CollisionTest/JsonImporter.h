#pragma once
#include <vector>
#include <fstream>
#include <iostream>

#include "EnvironmentActiveObject.h"
#include "AnimationFrameInfo.h"
#include "Map.h"
#include "SpriteManager.h"

using Json = nlohmann::json;
void from_json(const Json& j, Rectf& rect);
void from_json(const Json& j, AnimationFrameInfo& anFrame);
EnvironmentActiveObject::EnvironmentObjectType StringToType(const std::string& typeStr);

class JsonImporter
{
public:
	void ImportEnvironmentObjects(const std::string& jsonPath, Map& gameMap, SpriteManager& spriteManager) const;
	std::vector<AnimationFrameInfo> ImportAnimationFrameObjects(const std::string& jsonPath) const;
private:
	Json ParseJsonFile(const std::string& jsonPath) const;
};

