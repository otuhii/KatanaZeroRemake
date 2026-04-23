#include "pch.h"
#include "UserUtils.h"

#include "utils.h"

bool UserUtils::IsPolyInRectRaycast(const std::vector<Vector2f>& poly, const Rectf& rect)
{
	utils::HitInfo hitInfo{};

	if (utils::Raycast(
		poly,
		Vector2f{ rect.left, rect.bottom + rect.height },
		Vector2f{ rect.left, rect.bottom },
		hitInfo
	))
	{
		return true;
	}

	if (utils::Raycast(
		poly,
		Vector2f{ rect.left + rect.width,rect.bottom + rect.height },
		Vector2f{ rect.left + rect.width,rect.bottom }
		, hitInfo
	))
	{
		return true;
	}

	if (utils::Raycast(
		poly,
		Vector2f{ rect.left, rect.bottom },
		Vector2f{ rect.left + rect.width, rect.bottom }
		, hitInfo
	))
	{
		return true;
	}

	if (utils::Raycast(
		poly,
		Vector2f{ rect.left, rect.bottom + rect.height },
		Vector2f{ rect.left + rect.width, rect.bottom + rect.height },
		hitInfo
	))
	{
		return true;
	}


	return false;
}

bool UserUtils::IsPolyInRectAABB(const std::vector<Vector2f>& poly, const Rectf& rect)
{
	for (const Vector2f& vertex : poly)
	{
		if (utils::IsPointInRect(vertex, rect))
		{
			return true;
		}
	}

	return false;
}
