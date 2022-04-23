#include "pchheader.h"
#include "BezierCurve.h"

namespace Horyzen::Geo {

	BezierCurve::BezierCurve()
		: AbstractCurve(0, 1)
	{
//		m_ControlPoints.push_back({ 4 + 0,        4 + 0.1123,  -1, 1 });
//		m_ControlPoints.push_back({ 4 + 3.1213,   4 + 1.1675,  -1, 1.1 });
////		m_ControlPoints.push_back({ 4 + 2.0,      4 + 4.51235, -1, 1 });
//		m_ControlPoints.push_back({ 4 + 3.8,      4 + 4.51235, -1, 1 });
		//m_ControlPoints.push_back({ 4, 2, -1, 1 });
		//m_ControlPoints.push_back({ 3, 0, -1, 1 });
		//m_ControlPoints.push_back({ 4, 0, -1, 1 });
		//m_ControlPoints.push_back({ 5, 0, -1, 1 });


		//m_ControlPoints.push_back({ 0.0, 0.0, -1, 1 });
		//m_ControlPoints.push_back({ 0.5, 2.0, -1, 2 });
		////m_ControlPoints.push_back({ 1.0, 2.5, -1, 1 });
		////m_ControlPoints.push_back({ 2.5, 0.0, -1, 1 });
		//m_ControlPoints.push_back({ 3.0, 2.0, -1, 1 });

		//m_ControlPoints.push_back({ 0.0, 0.0, -1, 1.0 });
		//m_ControlPoints.push_back({ 0.0, 2.0, -1, 1.3 });
		//m_ControlPoints.push_back({ 4.0, 3.0, -1, 1.1 });


		//m_ControlPoints.push_back({ 0.0, 0.0, -1, 1 });
		//m_ControlPoints.push_back({ 0.5, 2.0, -1, 1 });
		//m_ControlPoints.push_back({ 3.0, 2.5, -1, 1 });
		//m_ControlPoints.push_back({ 4.5, 4.0, -1, 1 });

		m_ControlPoints.push_back({ 0.0, 0.0, -1, 1 });
		m_ControlPoints.push_back({ 0.5, 2.0, -1, 1 });
		m_ControlPoints.push_back({ 3.0, 3.5, -1, 1 });
		//	m_ControlPoints.push_back({ 4.5, 4.0, -1, 1 });

		m_ControlPoints.push_back({ 3.5, 0.0, -1, 1 });


//		m_ControlPoints.push_back({ 0.0, 0.0, -1, 1 });
//		m_ControlPoints.push_back({ 5.0, 0.0, -1, 1 });
//		m_ControlPoints.push_back({ 5.0, 5.0, -1, 1 });
//		m_ControlPoints.push_back({ 0.0, 7.0, -1, 1 });
////		m_ControlPoints.push_back({ 0.0, 0.0, -1, 1 });
	}

	BezierCurve::BezierCurve(const std::vector<Vec4D>& p_controlPoints,
		                     const f64                 p_tMin,
		                     const f64                 p_tMax)
		: AbstractCurve(p_tMin, p_tMax)
		, m_ControlPoints(p_controlPoints)
	{}

	std::pair<bool, std::vector<Vec4D>&> BezierCurve::GetControlPoints()
	{
		return { true, m_ControlPoints };
	}

}
