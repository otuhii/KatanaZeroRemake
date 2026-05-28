#pragma once
#include "ControlPoint.h"

class PathFinder final
{
public:
	static std::vector<int> CalculatePathToFloor(int currentIndex, int targetFloor, int currentFloor, const std::vector<ControlPoint>& controlPoints);

	static int GetNextControlPointIdxByType(int currentIndex, bool includeCurrentIndex, int currentFloor, ControlPoint::ControlPointType type, const std::vector<ControlPoint>& controlPoints);
	static int GetIndexByPointId(int id, const std::vector<ControlPoint>& controlPoints);
};

