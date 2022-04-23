#include "pchheader.h"
#include "NURBSSurfaceBuilder.h"

namespace Horyzen::Geo {

	NURBSSurfaceBuilder::NURBSSurfaceBuilder(std::shared_ptr<NURBSSurface> p_surfaceObject)
		: AbstractSurfaceBuilder(p_surfaceObject),
		  m_Surface(p_surfaceObject),
		  m_NT(p_surfaceObject->m_tControlPointsCount),
		  m_NS(p_surfaceObject->m_sControlPointsCount),
		  m_LeftT(p_surfaceObject->m_DegreeT + 1),
		  m_RightT(p_surfaceObject->m_DegreeT + 1),
		  m_LeftS(p_surfaceObject->m_DegreeS + 1),
		  m_RightS(p_surfaceObject->m_DegreeS + 1)
	{
		const u64 pT = p_surfaceObject->m_DegreeT;
		const u64 pS = p_surfaceObject->m_DegreeS;
		m_DersT.resize(3 * (pT + 1));
		m_DersS.resize(3 * (pS + 1));
		m_nduT.resize((pT + 1) * (pT + 1));
		m_nduS.resize((pS + 1) * (pS + 1));
		m_aT.resize(2 * (pT + 1));
		m_aS.resize(2 * (pS + 1));
	}

	Vec3D NURBSSurfaceBuilder::CalculatePoint(f64 p_t, f64 p_s)
	{
		u64 n = m_Surface->m_tControlPointsCount - 1;
		u64 pT = m_Surface->m_DegreeT;
		u64 m = m_Surface->m_sControlPointsCount - 1;
		u64 pS = m_Surface->m_DegreeS;

		auto& P = m_Surface->m_ControlPoints;

		auto spanTS = FindSpan(p_t, p_s);
		BasisFuns(spanTS.first, p_t, spanTS.second, p_s);

		Vec3D S;
		f64 weight = 0;
		for (size_t i = 0; i <= pT; ++i) {
			for (size_t j = 0; j <= pS; ++j) {
				S.x += m_NT[i] * m_NS[j] * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].x * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].w;
				S.y += m_NT[i] * m_NS[j] * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].y * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].w;
				S.z += m_NT[i] * m_NS[j] * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].z * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].w;
				weight += m_NT[i] * m_NS[j] * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].w;
			}
		}
		S.x /= weight;
		S.y	/= weight;
		S.z	/= weight;

		return S;
	}

	std::pair<Vec3D, Vec3D> NURBSSurfaceBuilder::CalculateFirstDerivative(f64 p_t, f64 p_s)
	{
		const u64 pT = m_Surface->m_DegreeT;
		const u64 pS = m_Surface->m_DegreeS;
		const std::vector<f64>& UT = m_Surface->m_KnotsT;
		const std::vector<f64>& US = m_Surface->m_KnotsS;

		const auto& P = m_Surface->m_ControlPoints;
		const u64 n = m_Surface->m_tControlPointsCount - 1;
		const u64 m = m_Surface->m_sControlPointsCount - 1;

		auto spanTS = FindSpan(p_t, p_s);

		BasisFunsFirstDerivatives(spanTS.first, p_t, spanTS.second, p_s);

#		define _dersT(i, j) m_DersT[(i) * (pT+1) + (j)]
#		define _dersS(i, j) m_DersS[(i) * (pS+1) + (j)]

		
		Vec4D C0 = { 0, 0, 0, 0 };

		for (size_t i = 0; i <= pT; ++i) {
			for (size_t j = 0; j <= pS; ++j) {
				C0.x += _dersT(0, i) * _dersS(0, j) * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].x * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].w;
				C0.y += _dersT(0, i) * _dersS(0, j) * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].y * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].w;
				C0.z += _dersT(0, i) * _dersS(0, j) * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].z * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].w;
				C0.w += _dersT(0, i) * _dersS(0, j) * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].w;
			}
		}

		Vec4D C1T = { 0, 0, 0, 0 };
		
		for (size_t i = 0; i <= pT; ++i) {
			for (size_t j = 0; j <= pS; ++j) {
				C1T.x += _dersT(1, i) * _dersS(0, j) * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].x * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].w;
				C1T.y += _dersT(1, i) * _dersS(0, j) * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].y * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].w;
				C1T.z += _dersT(1, i) * _dersS(0, j) * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].z * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].w;
				C1T.w += _dersT(1, i) * _dersS(0, j) * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].w;
			}
		}

		Vec4D C1S = { 0, 0, 0, 0 };

		for (size_t i = 0; i <= pT; ++i) {
			for (size_t j = 0; j <= pS; ++j) {
				C1S.x += _dersT(0, i) * _dersS(1, j) * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].x * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].w;
				C1S.y += _dersT(0, i) * _dersS(1, j) * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].y * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].w;
				C1S.z += _dersT(0, i) * _dersS(1, j) * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].z * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].w;
				C1S.w += _dersT(0, i) * _dersS(1, j) * P[(spanTS.first - pT + i) * (m + 1) + (spanTS.second - pS + j)].w;
			}
		}

		Vec3D o_derivativeT;
		o_derivativeT.x = (C0.w * C1T.x - C1T.w * C0.x) / (C0.w * C0.w);
		o_derivativeT.y = (C0.w * C1T.y - C1T.w * C0.y) / (C0.w * C0.w);
		o_derivativeT.z = (C0.w * C1T.z - C1T.w * C0.z) / (C0.w * C0.w);

		Vec3D o_derivativeS;
		o_derivativeS.x = (C0.w * C1S.x - C1S.w * C0.x) / (C0.w * C0.w);
		o_derivativeS.y = (C0.w * C1S.y - C1S.w * C0.y) / (C0.w * C0.w);
		o_derivativeS.z = (C0.w * C1S.z - C1S.w * C0.z) / (C0.w * C0.w);

		return std::make_pair(o_derivativeT, o_derivativeS);

#		undef _dersT
#		undef _dersS
	}

	std::pair<u64, u64> NURBSSurfaceBuilder::FindSpan(f64 p_t, f64 p_s)
	{
		std::pair<u64, u64> o_outMid;

		u64 n = m_Surface->m_tControlPointsCount - 1;
		u64 pT = m_Surface->m_DegreeT;
		auto& U = m_Surface->m_KnotsT;
		if (p_t == U[n + 1]) {
			o_outMid.first = n;
		}
		else {
			u64 lowT = pT;
			u64 highT = n + 1;
			u64 midT = (lowT + highT) / 2;
			while (p_t < U[midT] || p_t >= U[midT + 1]) {
				if (p_t < U[midT]) highT = midT;
				else              lowT = midT;
				midT = (lowT + highT) / 2;
			}
			o_outMid.first = midT;
		}

		u64 m = m_Surface->m_sControlPointsCount - 1;
		u64 pS = m_Surface->m_DegreeS;
		auto& V = m_Surface->m_KnotsS;
		if (p_s == V[m + 1]) {
			o_outMid.second = m;
		}
		else {
			u64 lowS = pS;
			u64 highS = m + 1;
			u64 midS = (lowS + highS) / 2;
			while (p_s < V[midS] || p_s >= V[midS + 1]) {
				if (p_s < V[midS]) highS = midS;
				else              lowS = midS;
				midS = (lowS + highS) / 2;
			}
			o_outMid.second = midS;
		}

		return o_outMid;
	}

	void NURBSSurfaceBuilder::BasisFuns(u64 p_i, f64 p_t, u64 p_j, f64 p_s)
	{
		{
			u64 pT = m_Surface->m_DegreeT;
			auto& U = m_Surface->m_KnotsT;

			m_NT[0] = 1.0;
			for (size_t k = 1; k <= pT; ++k) {
				m_LeftT[k] = p_t - U[p_i + 1 - k];
				m_RightT[k] = U[p_i + k] - p_t;
				f64 saved = 0.0;
				for (size_t r = 0; r < k; ++r) {
					f64 temp = m_NT[r] / (m_RightT[r + 1] + m_LeftT[k - r]);
					m_NT[r] = saved + m_RightT[r + 1] * temp;
					saved = m_LeftT[k - r] * temp;
				}
				m_NT[k] = saved;
			}
		}

		{
			u64 pS = m_Surface->m_DegreeS;
			auto& V = m_Surface->m_KnotsS;

			m_NS[0] = 1.0;
			for (size_t k = 1; k <= pS; ++k) {
				m_LeftS[k] = p_s - V[p_j + 1 - k];
				m_RightS[k] = V[p_j + k] - p_s;
				f64 saved = 0.0;
				for (size_t r = 0; r < k; ++r) {
					f64 temp = m_NS[r] / (m_RightS[r + 1] + m_LeftS[k - r]);
					m_NS[r] = saved + m_RightS[r + 1] * temp;
					saved = m_LeftS[k - r] * temp;
				}
				m_NS[k] = saved;
			}
		}
	}

	void NURBSSurfaceBuilder::BasisFunsFirstDerivatives(u64 p_i, f64 p_t, u64 p_j, f64 p_s)
	{
		const u64 pT = m_Surface->m_DegreeT;
		const std::vector<f64>& UT = m_Surface->m_KnotsT;
		

#		define _nduT(i, j)  m_nduT[(i) * (pT+1) + (j)]
#		define _dersT(i, j) m_DersT[(i) * (pT+1) + (j)]
#		define _aT(i, j)    m_aT[(i) * 2 + (j)]

		_nduT(0, 0) = 1.0;
		for (u64 j = 1; j <= pT; ++j) {
			m_LeftT[j] = p_t - UT[p_i + 1 - j];
			m_RightT[j] = UT[p_i + j] - p_t;
			f64 saved = 0.0;
			for (u64 r = 0; r < j; ++r) {
				_nduT(j, r) = m_RightT[r + 1] + m_LeftT[j - r];
				const f64 temp = _nduT(r, j - 1) / _nduT(j, r);

				_nduT(r, j) = saved + m_RightT[r + 1] * temp;
				saved = m_LeftT[j - r] * temp;
			}
			_nduT(j, j) = saved;
		}

		//..................................................
		// Loading the basis functions
		for (i64 j = 0; j <= pT; ++j) {
			_dersT(0, j) = _nduT(j, pT);
		}

		//..................................................
		// This section computes the derivatives
		for (i64 r = 0; r <= pT; ++r) {
			i64 s1 = 0;
			i64 s2 = 1;
			_aT(0, 0) = 1.0;

			f64 d = 0.0;
			i64 r1 = r - 1;
			i64 p1 = pT - 1;

			if (r >= 1) {
				_aT(s2, 0) = _aT(s1, 0) / _nduT(p1 + 1, r1);
				d = _aT(s2, 0) * _nduT(r1, p1);
			}

			i64 j1, j2;

			if (r1 >= -1) j1 = 1;
			else          j1 = -r1;

			if (r - 1 <= p1) j2 = 0;
			else             j2 = pT - r;

			for (i64 j = j1; j <= j2; ++j) {
				_aT(s2, j) = (_aT(s1, j) - _aT(s1, j - 1)) / _nduT(p1 + 1, r1 + j);
				d += _aT(s2, j) * _nduT(r1 + j, p1);
			}

			if (r <= p1) {
				_aT(s2, 1) = -_aT(s1, 0) / _nduT(p1 + 1, r);
				d += _aT(s2, 1) * _nduT(r, p1);
			}

			_dersT(1, r) = d;
			const i64 tmp = s1;
			s1 = s2;
			s2 = tmp;
		}

		//..................................................
		// Multiply through by the correct factors
		u64 r = pT;
		for (u64 j = 0; j <= pT; ++j) {
			_dersT(1, j) *= r;
		}

#		undef _nduT
#		undef _dersT
#		undef _aT

		const u64 pS = m_Surface->m_DegreeS;
		const std::vector<f64>& US = m_Surface->m_KnotsS;

#		define _nduS(i, j)  m_nduS[(i) * (pS+1) + (j)]
#		define _dersS(i, j) m_DersS[(i) * (pS+1) + (j)]
#		define _aS(i, j)    m_aS[(i) * 2 + (j)]

		/////////////////////////////////////////////////////////////////////////////////////////
		_nduS(0, 0) = 1.0;
		for (u64 j = 1; j <= pS; ++j) {
			m_LeftS[j] = p_s - US[p_j + 1 - j];
			m_RightS[j] = US[p_j + j] - p_s;
			f64 saved = 0.0;
			for (u64 r = 0; r < j; ++r) {
				_nduS(j, r) = m_RightS[r + 1] + m_LeftS[j - r];
				const f64 temp = _nduS(r, j - 1) / _nduS(j, r);

				_nduS(r, j) = saved + m_RightS[r + 1] * temp;
				saved = m_LeftS[j - r] * temp;
			}
			_nduS(j, j) = saved;
		}

		//..................................................
		// Loading the basis functions
		for (i64 j = 0; j <= pS; ++j) {
			_dersS(0, j) = _nduS(j, pS);
		}

		//..................................................
		// This section computes the derivatives
		for (i64 r = 0; r <= pS; ++r) {
			i64 s1 = 0;
			i64 s2 = 1;
			_aS(0, 0) = 1.0;

			f64 d = 0.0;
			i64 r1 = r - 1;
			i64 p1 = pS - 1;

			if (r >= 1) {
				_aS(s2, 0) = _aS(s1, 0) / _nduS(p1 + 1, r1);
				d = _aS(s2, 0) * _nduS(r1, p1);
			}

			i64 j1, j2;

			if (r1 >= -1) j1 = 1;
			else          j1 = -r1;

			if (r - 1 <= p1) j2 = 0;
			else             j2 = pS - r;

			for (i64 j = j1; j <= j2; ++j) {
				_aS(s2, j) = (_aS(s1, j) - _aS(s1, j - 1)) / _nduS(p1 + 1, r1 + j);
				d += _aS(s2, j) * _nduS(r1 + j, p1);
			}

			if (r <= p1) {
				_aS(s2, 1) = -_aS(s1, 0) / _nduS(p1 + 1, r);
				d += _aS(s2, 1) * _nduS(r, p1);
			}

			_dersS(1, r) = d;
			const i64 tmp = s1;
			s1 = s2;
			s2 = tmp;
		}

		//..................................................
		// Multiply through by the correct factors
		r = pS;
		for (u64 j = 0; j <= pS; ++j) {
			_dersS(1, j) *= r;
		}

#		undef _nduS
#		undef _dersS
#		undef _aS
	}

	


}