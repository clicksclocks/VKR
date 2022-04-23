#include "pchheader.h"
#include "NURBSSurfaceBuilder2.h"

namespace Horyzen::Geo {

	NURBSSurfaceBuilder2::NURBSSurfaceBuilder2(std::shared_ptr<NURBSSurface> p_surfaceObject)
		: AbstractSurfaceBuilder(p_surfaceObject)
		, m_Surface(p_surfaceObject)
		, m_NT(p_surfaceObject->m_tControlPointsCount)
		, m_NS(p_surfaceObject->m_sControlPointsCount)
		, m_LeftT(p_surfaceObject->m_DegreeT + 1)
		, m_RightT(p_surfaceObject->m_DegreeT + 1)
		, m_LeftS(p_surfaceObject->m_DegreeS + 1)
		, m_RightS(p_surfaceObject->m_DegreeS + 1)
		, m_PointBuffer((p_surfaceObject->m_DegreeT + 1)*(p_surfaceObject->m_DegreeS + 1))
	    , m_WeightBuffer((p_surfaceObject->m_DegreeT + 1)*(p_surfaceObject->m_DegreeS + 1))
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

	Vec3D NURBSSurfaceBuilder2::CalculatePoint(f64 p_t, f64 p_s)
	{
		f64 d1, d2;
		u64 kT = m_Surface->m_DegreeT + 1;
		u64 kS = m_Surface->m_DegreeS + 1;
		u64 n = m_Surface->m_tControlPointsCount - 1;
		u64 m = m_Surface->m_sControlPointsCount - 1;
		auto& P = m_Surface->m_ControlPoints;
		const auto& U = m_Surface->m_KnotsT;
		const auto& V = m_Surface->m_KnotsS;

		auto spanTS = FindSpan(p_t, p_s);

#		define _P(i, j) P[(i) * (m+1) + (j)]
#		define _PB(i, j) m_PointBuffer[(i) * kS + (j)]
#		define _WB(i, j) m_WeightBuffer[(i) * kS + (j)]

		for (size_t i = 0; i < kT; ++i) {
			for (size_t j = 0; j < kS; ++j) {
				_PB(i, j) = _P(spanTS.first - kT + 1 + i, spanTS.second - kS + 1 + j).xyz().scaled(_P(spanTS.first - kT + 1 + i, spanTS.second - kS + 1 + j).w);
				_WB(i, j) = _P(spanTS.first - kT + 1 + i, spanTS.second - kS + 1 + j).w;
			}
		}

		for (size_t j = 0; j < kS; ++j) {
			for (size_t r = 1; r < kT; ++r) {
				for (size_t i = kT - 1; i >= r; --i) {
					const u64 k = spanTS.first - kT + 1 + i;
					const f64 d1 = p_t - U[k];
					const f64 d2 = U[k + kT - r] - p_t;

					_PB(i, j) = (_PB(i, j).scaled(d1) + _PB(i - 1, j).scaled(d2)).scaled(1.0 / (d1 + d2));
					_WB(i, j) = (_WB(i, j) * d1       + _WB(i - 1, j) * d2      ) / (d1 + d2);
				}
			}
		}

		for (size_t i = 0; i < kT; ++i) {
			for (size_t r = 1; r < kS; ++r) {
				for (size_t j = kS - 1; j >= r; --j) {
					const u64 k = spanTS.second - kS + 1 + j;
					const f64 d1 = p_s - V[k];
					const f64 d2 = V[k + kS - r] - p_s;

					_PB(i, j) = (_PB(i, j).scaled(d1) + _PB(i, j - 1).scaled(d2)).scaled(1.0 / (d1 + d2));
					_WB(i, j) = (_WB(i, j) * d1       + _WB(i, j - 1) * d2      ) / (d1 + d2);
				}
			}
		}

		return _PB(kT - 1, kS - 1).scaled(1.0 / _WB(kT - 1, kS - 1));

#		undef _P 
#		undef _PB
#		undef _WB
	}

	std::pair<u64, u64> NURBSSurfaceBuilder2::FindSpan(f64 p_t, f64 p_s)
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

}
