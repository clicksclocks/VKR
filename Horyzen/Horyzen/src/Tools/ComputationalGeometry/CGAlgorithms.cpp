#include "pchheader.h"
#include "CGAlgorithms.h"

#include "Horyzen/Core.h"

std::vector<Vec2D> CGAlgorithms::FindConvexHull2DJarvisMarch(const std::vector<Vec2D>& p_points)
{
	std::vector<Vec2D> P(p_points);

	//.................................................
	// Finding point with min Y coordinate
	f64 minY = P[0].y;
	u64 minYIndex = 0;
	for (size_t i = 1; i < P.size(); ++i) {
		if ((P[i].y < minY) ||
			(P[i].y == minY && P[i].x < P[minYIndex].x)) {
			minY = P[i].y;
			minYIndex = i;
		}
	}

	std::vector<Vec2D> o_convexHullPoints;
	o_convexHullPoints.push_back(P[minYIndex]);
	
	Vec2D v{ 1.0, 0.0 };
	u64 prevIndex = minYIndex;
	while (true) {
		u64 bestNextIndex;
		f64 bestCosAngle = -2.0;
		for (size_t i = 0; i < P.size(); ++i) {
			if (i == prevIndex) continue;
			Vec2D w = P[i] - P[prevIndex];
			const f64 cosAngle = Vec2D::CosOfAngleBetween(v, w);
			if (cosAngle > bestCosAngle) {
				bestCosAngle = cosAngle;
				bestNextIndex = i;
			}
		}
		if (bestNextIndex == minYIndex) break;
		o_convexHullPoints.push_back(P[bestNextIndex]);
		v = P[bestNextIndex] - P[prevIndex];
		prevIndex = bestNextIndex;
		
	}
	return o_convexHullPoints;


	////..................................................
	//// Moving min Y point to the beginning of the array
	//std::swap(P[0], P[minYIndex]);

	//const Vec2D p0 = P[0];
	//auto comparator = [p0](const Vec2D& p_first, const Vec2D& p_second) {
	//	const Vec2D v1 = p_first - p0;
	//	const Vec2D v2 = p_second - p0;
	//	const f64 cross = v1.x * v2.y - v2.x * v1.y;
	//	if (cross < 0.0) return false;
	//	if (cross > 0.0) return true;
	//	//................................
	//	// Here what happens if cross = 0
	//	if ((p_first.x - p_second.x) == 0.0) {
	//		return p_first.y < p_second.y ? false : true;
	//	} else {
	//		return p_first.x < p_second.x ? false : true;
	//	}
	//};

	////..................................................
	//// Sorting array by angle
	//std::sort(P.begin() + 1, P.end(), comparator);



}
