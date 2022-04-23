#pragma once

#include "Horyzen/Core.h"

#include "Horyzen/Math/Vec2.h"

class HORYZEN_OBJECT CGAlgorithms
{
public:

	static std::vector<Vec2D> FindConvexHull2DJarvisMarch(const std::vector<Vec2D>& p_points);

};

