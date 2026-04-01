#include "pch.h"
#include "UserUtils.h"

//return angle between two vector without angle restriction using dot and cross formula
float UserUtils::AngleBetweenVectors(const Vector2f& v1, const Vector2f& v2)
{
	return atan2f(v1.x * v2.y - v1.y * v2.x, v1.x * v2.x + v1.y * v2.y);
}
