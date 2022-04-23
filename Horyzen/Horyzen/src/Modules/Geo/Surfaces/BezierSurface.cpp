#include "pchheader.h"
#include "BezierSurface.h"

namespace Horyzen::Geo {

	BezierSurface::BezierSurface()
		: AbstractSurface(0, 1, 0, 1)
	{
		for (size_t i = 0; i < 7; ++i) {
			for (size_t j = 0; j < 9; ++j) {
				m_ControlPoints.push_back({ f64(j), 0, f64(i), 1 });
			}
		}
		m_tControlPointsCount = 7;
		m_sControlPointsCount = 9;
	}

	std::tuple<bool, std::vector<Vec4D>&, u64, u64> BezierSurface::GetControlPoints()
	{
		return std::tuple<bool, std::vector<Vec4D>&, u64, u64>(true, 
			                                                   m_ControlPoints,
			                                                   m_tControlPointsCount,
			                                                   m_sControlPointsCount);
	}

}
