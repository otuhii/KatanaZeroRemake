#pragma once

#include <vector>

namespace UserUtils {
	bool IsPolyInRect(const std::vector<Vector2f>& poly, const Rectf& rect);



	//i dont use concaves sooo https://www.metanetsoftware.com/technique/tutorialA.html 
	//bool IsOverlapping(const std::vector<Vector2f>& poly1, const std::vector<Vector2f>& poly2);
    
}
