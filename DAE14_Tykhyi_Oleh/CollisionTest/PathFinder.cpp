#include "pch.h"
#include "PathFinder.h"

//BFS search for path to target floor
std::vector<int> PathFinder::CalculatePathToFloor(int currentIndex, int targetFloor, int currentFloor, const std::vector<ControlPoint>& controlPoints)
{
	if (controlPoints.empty())
	{
		return {};
	}

	std::vector<int> parentIndices(controlPoints.size(), -1);
	std::vector<bool> visited(controlPoints.size(), false);

	std::vector<int> queue;

	int
		head{ 0 };

	int startIndex{ GetNextControlPointIdxByType(
		currentIndex,
		currentFloor,
		ControlPoint::ControlPointType::stairSignifier,
		controlPoints
	) };

	if (startIndex == -1)
	{
		return {};
	}

	queue.push_back(startIndex);
	visited[startIndex] = true;

	int reachedTargetIndex{ -1 };

	while (head < static_cast<int>(queue.size()))
	{
		int currentIndex{ queue[head] };
		head++;

		if (controlPoints[currentIndex].floor == targetFloor)
		{
			reachedTargetIndex = currentIndex;
			break;
		}

		for (int neighborPointId : controlPoints[currentIndex].neighborIds)
		{
			int 
				neighborIndex{ GetIndexByPointId(neighborPointId, controlPoints) };

			if (neighborIndex != -1 && !visited[neighborIndex])
			{
				visited[neighborIndex] = true;
				parentIndices[neighborIndex] = currentIndex;
				queue.push_back(neighborIndex);
			}
		}
	}

	std::vector<int> path;

	if (reachedTargetIndex != -1)
	{
		int currentQueueIndex{ reachedTargetIndex };

		while (currentQueueIndex != -1)
		{
			path.push_back(currentQueueIndex);

			if (currentQueueIndex == startIndex)
			{
				break;
			}

			currentQueueIndex = parentIndices[currentQueueIndex];
		}
	}

	return path;
}

int PathFinder::GetNextControlPointIdxByType(int currentIndex, int currentFloor, ControlPoint::ControlPointType type, const std::vector<ControlPoint>& controlPoints)
{
	for (size_t index{ 1 }; index < controlPoints.size(); ++index)
	{
		size_t searchIndex{ (currentIndex + index) % controlPoints.size() };

		if (controlPoints[searchIndex].floor == currentFloor && controlPoints[searchIndex].type == type)
		{
			return static_cast<int>(searchIndex);
		}
	}
	return -1;
}

int PathFinder::GetIndexByPointId(int id, const std::vector<ControlPoint>& controlPoints)
{
	//i could use map but fuck that, not that much points for O(n)
	for (size_t index{ 0 }; index < controlPoints.size(); ++index)
	{
		if (controlPoints[index].pointId == id)
		{
			return static_cast<int>(index);
		}
	}

	return -1;
}
