#include "pchheader.h"
#include "NURBSCurveBuilder2.h"

namespace Horyzen::Geo {

	NURBSCurveBuilder2::NURBSCurveBuilder2(std::shared_ptr<NURBSCurve> p_curveObject)
		: AbstractCurveBuilder(p_curveObject)
		, m_Curve(p_curveObject)
		, m_PointBuffer(p_curveObject->m_Degree+1)
		, m_WeightBuffer(p_curveObject->m_Degree+1)
	{
		m_N.resize(p_curveObject->m_ControlPoints.size());
		m_Left.resize(p_curveObject->m_Degree + 1);
		m_Right.resize(p_curveObject->m_Degree + 1);
	}

	Vec3D NURBSCurveBuilder2::CalculatePoint(f64 p_t)
	{
		f64 d1, d2;
		const u64 k = m_Curve->m_Degree + 1;
		const auto& P = m_Curve->m_ControlPoints;
		const auto& U = m_Curve->m_Knots;

		const u64 l = FindSpan(p_t);

		for (size_t j = 0; j < k; ++j) {
			m_PointBuffer[j] = P[l - k + 1 + j].xyz() * P[l - k + 1 + j].w;
			m_WeightBuffer[j] = P[l - k + 1 + j].w;
		}

		for (size_t r = 1; r < k; ++r) {
			for (size_t j = k - 1; j >= r; --j) {
				size_t i = l - k + 1 + j;
				const f64 d1 = p_t - U[i];
				const f64 d2 = U[i + k - r] - p_t;
				m_PointBuffer[j] = (m_PointBuffer[j].scaled(d1) + m_PointBuffer[j - 1].scaled(d2)).scaled(1.0 / (d1 + d2));
				m_WeightBuffer[j] = (m_WeightBuffer[j] * d1 + m_WeightBuffer[j - 1] * d2) / (d1 + d2);
			}
		}

		return m_PointBuffer[k - 1].scaled(1.0 / m_WeightBuffer[k - 1]);
	}

	u64 NURBSCurveBuilder2::FindSpan(f64 p_t)
	{
		u64 n = m_Curve->m_ControlPoints.size() - 1;
		u64 p = m_Curve->m_Degree;
		auto& U = m_Curve->m_Knots;
		if (p_t == U[n + 1]) return n;
		u64 low = p;
		u64 high = n + 1;
		u64 mid = (low + high) / 2;
		while (p_t < U[mid] || p_t >= U[mid + 1]) {
			if (p_t < U[mid]) high = mid;
			else              low = mid;
			mid = (low + high) / 2;
		}
		return mid;
	}

}