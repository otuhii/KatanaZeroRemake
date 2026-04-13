#include "pch.h"
#include "UserUtils.h"

#include "utils.h"

bool UserUtils::IsPolyInRect(const std::vector<Vector2f>& poly, const Rectf& rect)
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
