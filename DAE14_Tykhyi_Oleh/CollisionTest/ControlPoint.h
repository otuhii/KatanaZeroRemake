#pragma once
#include "Vector2f.h"

struct ControlPoint {
	enum class ControlPointType {
		none=-1,

		leadingPoint,
		stairSignifier
	};

	Vector2f position{};

	ControlPointType type{};
	int floor{};
};