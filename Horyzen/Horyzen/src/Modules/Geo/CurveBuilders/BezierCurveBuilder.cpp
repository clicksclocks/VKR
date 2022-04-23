#include "pchheader.h"
#include "BezierCurveBuilder.h"

namespace Horyzen::Geo {

	BezierCurveBuilder::BezierCurveBuilder(std::shared_ptr<BezierCurve> p_curveObject)
		: AbstractCurveBuilder(p_curveObject),
		  m_Curve(p_curveObject),
		  n(m_Curve->GetControlPoints().second.size() - 1),
		  m_Temp(n + 1),
		  m_BersteinValues(n + 1)
	{}

	Vec3D BezierCurveBuilder::CalculatePoint(f64 p_t)
	{
		Vec3D o_point;
		AllBernstein(p_t);

		auto& P = m_Curve->GetControlPoints().second;
		f64 weight = 0;
		for (size_t k = 0; k <=n; ++k) {
			o_point.x += P[k].x * P[k].w * m_BersteinValues[k];
			o_point.y += P[k].y * P[k].w * m_BersteinValues[k];
			o_point.z += P[k].z * P[k].w * m_BersteinValues[k];
			weight += P[k].w * m_BersteinValues[k];
		}
		o_point.x /= weight;
		o_point.y /= weight;
		o_point.z /= weight;

		return o_point;
	}

	void BezierCurveBuilder::AllBernstein(f64 t)
	{
		m_BersteinValues[0] = 1.0;
		f64 t1 = 1.0 - t;
		for (size_t j = 1; j <= n; ++j) {
			f64 saved = 0;
			for (size_t k = 0; k < j; ++k) {
				f64 temp = m_BersteinValues[k];
				m_BersteinValues[k] = saved + t1 * temp;
				saved = t * temp;
			}
			m_BersteinValues[j] = saved;
		}
	}

	f64 BezierCurveBuilder::Bernstein(u64 i, f64 t)
	{
		std::memset(m_Temp.data(), 0, m_Temp.size() * sizeof(f64));
		m_Temp[n - i] = 1.0;
		f64 t1 = 1.0 - t;
		for (size_t k = 1; k <= n; ++k) {
			for (size_t j = k; j >= k; j--) {
				m_Temp[j] = t1 * m_Temp[j] + t * m_Temp[j - 1];
			}
		}
		return m_Temp[n];
	}

}
