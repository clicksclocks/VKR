#include "pchheader.h"
#include "NURBSCurveBuilder.h"


namespace Horyzen::Geo {

	NURBSCurveBuilder::NURBSCurveBuilder(std::shared_ptr<NURBSCurve> p_curveObject)
		: AbstractCurveBuilder(p_curveObject),
		  m_Curve(p_curveObject)
	{
		const u64 p = p_curveObject->m_Degree;

		m_N.resize(p_curveObject->m_ControlPoints.size());
		m_Left.resize(p + 1);
		m_Right.resize(p + 1);
		m_Ders.resize(3 * (p + 1));
		m_ndu.resize((p + 1) * (p + 1));
		m_a.resize(2 * (p + 1));
	}

	Vec3D NURBSCurveBuilder::CalculatePoint(f64 p_t)
	{
		u64 n = m_Curve->m_ControlPoints.size() - 1;
		u64 p = m_Curve->m_Degree;
		auto& P = m_Curve->m_ControlPoints;
		u64 span = FindSpan(p_t);
		BasisFuns(span, p_t);
		Vec3D o_point;
		f64 weight = 0;
		for (size_t i = 0; i <= p; ++i) {
			o_point.x += m_N[i] * P[span - p + i].x * P[span - p + i].w;
			o_point.y += m_N[i] * P[span - p + i].y * P[span - p + i].w;
			o_point.z += m_N[i] * P[span - p + i].z * P[span - p + i].w;
			weight += m_N[i] * P[span - p + i].w;
		}
		o_point.x /= weight;
		o_point.y /= weight;
		o_point.z /= weight;

		return o_point;
	}

	Vec3D NURBSCurveBuilder::CalculateFirstDerivative(f64 p_t)
	{
		const u64 p = m_Curve->m_Degree;
		const auto& U = m_Curve->m_Knots;
		const auto& P = m_Curve->m_ControlPoints;
		const u64 n = m_Curve->m_ControlPoints.size() - 1;

		u64 span = FindSpan(p_t);
		BasisFunsFirstDerivatives(span, p_t);

#		define _ders(i, j) m_Ders[(i) * (p+1) + (j)]

		Vec4D C1 = { 0, 0, 0, 0 };
		Vec4D C0 = { 0, 0, 0, 0 };

		for (u64 j = 0; j <= p; ++j) {
			C0.x += _ders(0, j) * P[span - p + j].x * P[span - p + j].w;
			C0.y += _ders(0, j) * P[span - p + j].y * P[span - p + j].w;
			C0.z += _ders(0, j) * P[span - p + j].z * P[span - p + j].w;
			C0.w += _ders(0, j) * P[span - p + j].w;

			C1.x += _ders(1, j) * P[span - p + j].x * P[span - p + j].w;
			C1.y += _ders(1, j) * P[span - p + j].y * P[span - p + j].w;
			C1.z += _ders(1, j) * P[span - p + j].z * P[span - p + j].w;
			C1.w += _ders(1, j) * P[span - p + j].w;
		}

		Vec3D o_derivative;
		o_derivative.x = (C0.w * C1.x - C1.w * C0.x) / (C0.w * C0.w);
		o_derivative.y = (C0.w * C1.y - C1.w * C0.y) / (C0.w * C0.w);
		o_derivative.z = (C0.w * C1.z - C1.w * C0.z) / (C0.w * C0.w);
		
		return o_derivative;

#		undef _ders
	}

	u64 NURBSCurveBuilder::FindSpan(f64 p_t)
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

	void NURBSCurveBuilder::BasisFuns(u64 p_i, f64 p_t)
	{
		u64 p = m_Curve->m_Degree;
		auto& U = m_Curve->m_Knots;

		m_N[0] = 1.0;
		for (size_t j = 1; j <= p; ++j) {
			m_Left[j] = p_t - U[p_i + 1 - j];
			m_Right[j] = U[p_i + j] - p_t;
			f64 saved = 0.0;
			for (size_t r = 0; r < j; ++r) {
				f64 temp = m_N[r] / (m_Right[r + 1] + m_Left[j - r]);
				m_N[r] = saved + m_Right[r + 1] * temp;
				saved = m_Left[j - r] * temp;
			}
			m_N[j] = saved;
		}
	}

	void NURBSCurveBuilder::BasisFunsFirstDerivatives(u64 p_i, f64 p_t)
	{
		const u64 p = m_Curve->m_Degree;
		const std::vector<f64>& U = m_Curve->m_Knots;

#		define _ndu(i, j) m_ndu[(i) * (p+1) + (j)]
#		define _ders(i, j) m_Ders[(i) * (p+1) + (j)]
#		define _a(i, j) m_a[(i) * 2 + (j)]

		_ndu(0, 0) = 1.0;
		for (u64 j = 1; j <= p; ++j) {
			m_Left[j] = p_t - U[p_i + 1 - j];
			m_Right[j] = U[p_i + j] - p_t;
			f64 saved = 0.0;
			for (u64 r = 0; r < j; ++r) {
				_ndu(j, r) = m_Right[r + 1] + m_Left[j - r];
				const f64 temp = _ndu(r, j - 1) / _ndu(j, r);

				_ndu(r, j) = saved + m_Right[r + 1] * temp;
				saved = m_Left[j - r] * temp;
			}
			_ndu(j, j) = saved;
		}

		//..................................................
		// Loading the basis functions
		for (i64 j = 0; j <= p; ++j) {
			_ders(0, j) = _ndu(j, p);
		}

		//..................................................
		// This section computes the derivatives
		for (i64 r = 0; r <= p; ++r) {
			i64 s1 = 0;
			i64 s2 = 1;
			_a(0, 0) = 1.0;

			f64 d = 0.0;
			i64 r1 = r - 1;
			i64 p1 = p - 1;

			if (r >= 1) {
				_a(s2, 0) = _a(s1, 0) / _ndu(p1 + 1, r1);
				d = _a(s2, 0) * _ndu(r1, p1);
			}

			i64 j1, j2;

			if (r1 >= -1) j1 = 1;
			else          j1 = -r1;

			if (r - 1 <= p1) j2 = 0;
			else             j2 = p - r;

			for (i64 j = j1; j <= j2; ++j) {
				_a(s2, j) = (_a(s1, j) - _a(s1, j - 1)) / _ndu(p1 + 1, r1 + j);
				d += _a(s2, j) * _ndu(r1 + j, p1);
			}

			if (r <= p1) {
				_a(s2, 1) = -_a(s1, 0) / _ndu(p1 + 1, r);
				d += _a(s2, 1) * _ndu(r, p1);
			}

			_ders(1, r) = d;
			const i64 tmp = s1;
			s1 = s2;
			s2 = tmp;
		}

		//..................................................
		// Multiply through by the correct factors
		u64 r = p;
		for (u64 j = 0; j <= p; ++j) {
			_ders(1, j) *= r;
		}
	}

#		undef _ndu
#		undef _ders
#		undef _a

}
