#pragma once

#include <vector>

namespace UserUtils {
	bool IsPolyInRectRaycast(const std::vector<Vector2f>& poly, const Rectf& rect);
	bool IsPolyInRectAABB(const std::vector<Vector2f>& poly, const Rectf& rect);

	bool ArePolygonsOverlapping(const std::vector<Vector2f>& poly1, const std::vector<Vector2f>& poly2);

	Vector2f PredictNormalWithVelocity(const Vector2f& velocity);
	//i dont use concaves sooo https://www.metanetsoftware.com/technique/tutorialA.html 
	//bool IsOverlapping(const std::vector<Vector2f>& poly1, const std::vector<Vector2f>& poly2);
}
