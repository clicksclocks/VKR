#include "pchheader.h"
#include "NURBSCurve.h"

namespace Horyzen::Geo {

	NURBSCurve::NURBSCurve()
		: AbstractCurve(0, 1)
	{
		//m_ControlPoints.push_back({ 0, 0, -1, 1 });
		//m_ControlPoints.push_back({ 1, 3, -1, 1 });
		//m_ControlPoints.push_back({ 2, 4, -1, 1 });
		//m_ControlPoints.push_back({ 3, 3, -1, 1 });
		//m_ControlPoints.push_back({ 4, 0, -1, 1 });
		//m_ControlPoints.push_back({ 5, 2, -1, 1 });
		//m_ControlPoints.push_back({ 6, -1, -1, 1 });
		//m_ControlPoints.push_back({ 7, 0, -1, 1 });
		//m_ControlPoints.push_back({ 8, 3, -1, 1 });
		//m_ControlPoints.push_back({ 9, 2, -1, 1 });

		//m_ControlPoints.push_back({ 8, 4, -1, 1 });
		//m_ControlPoints.push_back({ 7, 5, -1, 1 });
		//m_ControlPoints.push_back({ 6, 3, -1, 1 });
		//m_ControlPoints.push_back({ 5, 3, -1, 1 });
		//m_ControlPoints.push_back({ 4, 1, -1, 1 });

		//m_ControlPoints.push_back({ 0.0, 0.0, -1, 1 });
		//m_ControlPoints.push_back({ 0.5, 2.0, -1, 1 });
		//m_ControlPoints.push_back({ 3.0, 1.0, -1, 1 });


		//m_ControlPoints.push_back({ 0.0, 0.0, -1, 1 });
		//m_ControlPoints.push_back({ 1.0, 0.0, -1, 1 });
		//m_ControlPoints.push_back({ 2.0, 4.1, -1, 1 });

		m_ControlPoints.push_back({ -4.0,  0.0, -1, 1 });
		m_ControlPoints.push_back({  0.0, 10.0, -1, 1 });
		m_ControlPoints.push_back({  4.0,  0.0, -1, 1 });
		m_ControlPoints.push_back({  6.0,  3.0, -1, 1 });
		m_ControlPoints.push_back({  7.0,  5.0, -1, 1 });

		//m_ControlPoints.push_back({ 0.0, 0.0, -1, 1 });
		//m_ControlPoints.push_back({ 0.6, 1.0, -1, 1 });
		//m_ControlPoints.push_back({ 1.1, 0.5, -1, 1 });
		//m_ControlPoints.push_back({ 1.5, 1.0, -1, 1 });

	//	m_ControlPoints.push_back({ 4.5, 4.0, -1, 1 });

		//m_ControlPoints.push_back({ 3.5, 7.0, -1, 1 });
		//m_ControlPoints.push_back({ 6.0, 2.0, -1, 1 });
		//m_ControlPoints.push_back({ 6.0, 5.0, -1, 1 });
	//	m_ControlPoints.push_back({ 3.0, 5.0, -1, 1 });


		m_Degree = 3;
		u64 p = m_Degree;
		u64 N = m_ControlPoints.size();
		u64 m = p + N;
		u64 S = N - p;
		m_Knots.resize(m_Degree + N + 1);
		for (size_t i = 0; i < m_Degree + 1; ++i) {
			m_Knots[i] = 0;
			m_Knots[m - i] = 1;
		}
		for (size_t i = 1; i < S; ++i) {
			m_Knots[p + i] = f64(i) / S;
		}

//		InsertKnot(0.5, 2);
	}

	NURBSCurve::NURBSCurve(const std::vector<Vec4D>& p_controlPoints,
		                   const std::vector<f64>&   p_knots,
		                   const u64                 p_degree,
		                   const f64                 p_tMin,
		                   const f64                 p_tMax)
		: AbstractCurve(p_tMin, p_tMax),
		  m_ControlPoints(p_controlPoints),
		  m_Knots(p_knots),
		  m_Degree(p_degree)
	{}



	std::pair<bool, std::vector<Vec4D>&> NURBSCurve::GetControlPoints()
	{
		return { true, m_ControlPoints };
	}

	void NURBSCurve::InsertKnot(f64 p_knot, u64 p_times)
	{
		u64 np = m_ControlPoints.size() - 1;
		u64 p = m_Degree;
		u64 mp = np + p + 1;
		u64 r = p_times;
		u64 nq = np + r;
		f64 u = p_knot;

		std::vector<f64>& UP = m_Knots;
		std::vector<f64> UQ(UP.size() + r);

		//.........................................
		// Defining k
		u64 k;
		for (size_t i = 0; i < UP.size() - 1; ++i) {
			if (u >= UP[i] && u < UP[i + 1]) {
				k = i;
				break;
			}
		}

		//.........................................
		// Loading new knot vector
		for (size_t i = 0; i <= k; ++i) UQ[i] = UP[i];
		for (size_t i = 1; i <= r; ++i) UQ[k + i] = u;
		for (size_t i = k + 1; i <= mp; ++i) UQ[i + r] = UP[i];

		//.........................................
		// Defining s
		u64 s = 0;
		for (size_t i = 0; i < UP.size(); ++i) {
			if (u == UP[i]) {
				s++;
			}
		}

		//.........................................
		// Saving unaltered control points
		std::vector<Vec4D>& Pw = m_ControlPoints;
		std::vector<Vec4D> Qw(nq + 1);
		std::vector<Vec4D> Rw(p + 1);
		for (size_t i = 0; i <= k - p; ++i) Qw[i] = Pw[i];
		for (size_t i = k - s; i <= np; ++i) Qw[i + r] = Pw[i];
		for (size_t i = 0; i <= p - s; ++i) Rw[i] = Pw[k - p + i];
		
		//.........................................
		// Inserting the knot r times
		u64 L;
		for (size_t j = 1; j <= r; ++j) {
			L = k - p + j;
			for (size_t i = 0; i <= p - j - s; ++i) {
				f64 alpha = (u - UP[L + i]) / (UP[i + k + 1] - UP[L + i]);
				Rw[i].x = alpha * Rw[i + 1].x + (1.0 - alpha) * Rw[i].x;
				Rw[i].y = alpha * Rw[i + 1].y + (1.0 - alpha) * Rw[i].y;
				Rw[i].z = alpha * Rw[i + 1].z + (1.0 - alpha) * Rw[i].z;
				Rw[i].w = alpha * Rw[i + 1].w + (1.0 - alpha) * Rw[i].w;
			}
			Qw[L] = Rw[0];
			Qw[k + r - j - s] = Rw[p - j - s];
		}

		//.........................................
		// Loading remaining control points
		for (size_t i = L + 1; i < k - s; ++i) {
			Qw[i] = Rw[i - L];
		}

		m_ControlPoints = std::move(Qw);
		m_Knots = std::move(UQ);
	}

	void NURBSCurve::DecomposeIntoBezier()
	{
		struct KnotInfo 
		{
			f64 value;
			u64 multiplicity;
			u64 k;
		};

		std::vector<KnotInfo> knotInfos;
		f64 currentValue = m_Knots[m_Knots.size() - 1];
		for (size_t i = 0; i < m_Knots.size(); ++i) {
			if (m_Knots[i] != currentValue) {
				knotInfos.push_back({ m_Knots[i] , 1, i });
				currentValue = m_Knots[i];
			} else {
				knotInfos[knotInfos.size() - 1].multiplicity++;
			}
		}

		u64 p = m_Degree;
		for (size_t i = 1; i < knotInfos.size() - 1; ++i) {
			InsertKnot(knotInfos[i].value, p - knotInfos[i].value);
		}
	}

	std::vector<std::vector<Vec4D>> NURBSCurve::GetBezierControlPoints()
	{
		auto tmpCurve = std::make_shared<NURBSCurve>(m_ControlPoints,
			                                         m_Knots,
			                                         m_Degree,
			                                         m_tMin,
			                                         m_tMax);

		tmpCurve->DecomposeIntoBezier();
		auto& points = tmpCurve->GetControlPoints().second;
		
		u64 index = 0;
		const u64 S = (points.size() - 1) / m_Degree;
		std::vector<std::vector<Vec4D>> o_bezierPatches(S, std::vector<Vec4D>(m_Degree+1));
		for (size_t i = 0; i < S; ++i) {
			auto& patch = o_bezierPatches[i];
			for (size_t k = 0; k <= m_Degree; ++k) {
				patch[k] = points[index];
				index++;
			}
			index--;
		}

		return o_bezierPatches;
	}

}
