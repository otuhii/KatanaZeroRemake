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

bool UserUtils::ArePolygonsOverlapping(const std::vector<Vector2f>& poly1, const std::vector<Vector2f>& poly2)
{
	utils::HitInfo hitinfo{};


	for (size_t index{ 0 }; index < poly1.size(); ++index)
	{
		Vector2f
			point1{ poly1[index] },
			point2{ poly1[(index + 1) % poly1.size()] };

		if (utils::Raycast(poly2, point1, point2, hitinfo))
		{
			return true;
		}
	}

	return false;
}

Vector2f UserUtils::PredictNormalWithVelocity(const Vector2f& velocity)
{
	if (std::abs(velocity.x) > std::abs(velocity.y))
	{
		if (velocity.x > 0)
		{
			return Vector2f{ -1.f, 0.f };
		}
		else
		{
			return Vector2f{ 1.f, 0.f };
		}
	}
	else
	{
		if (velocity.y > 0)
		{
			return Vector2f{ 0.f, -1.f };
		}
		else
		{
			return Vector2f{ 0.f, 1.f };
		}
	}
}
