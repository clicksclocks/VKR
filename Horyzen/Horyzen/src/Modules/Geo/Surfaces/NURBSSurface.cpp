#include "pchheader.h"
#include "NURBSSurface.h"

namespace Horyzen::Geo {

	NURBSSurface::NURBSSurface()
		: AbstractSurface(0, 1, 0, 1)
	{
		for (size_t i = 0; i < 6; ++i) {
			for (size_t j = 0; j < 6; ++j) {
				m_ControlPoints.push_back({ f64(j), 0, f64(i), 1 });
		//		m_ControlPoints.push_back({ f64(j), std::sin(i * j), f64(i), 1 });
			}
		}
		m_tControlPointsCount = 6;
		m_sControlPointsCount = 6;

		m_DegreeT = 3;
		m_DegreeS = 3;

		u64 pT = m_DegreeT;
		u64 pS = m_DegreeS;

		u64 NT = m_tControlPointsCount;
		u64 NS = m_sControlPointsCount;

		u64 mT = pT + NT;
		u64 mS = pS + NS;
		u64 ST = NT - pT;
		u64 SS = NS - pS;

		m_KnotsT.resize(m_DegreeT + NT + 1);
		for (size_t i = 0; i < m_DegreeT + 1; ++i) {
			m_KnotsT[i] = 0;
			m_KnotsT[mT - i] = 1;
		}
		for (size_t i = 1; i < ST; ++i) {
			m_KnotsT[pT + i] = f64(i) / ST;
		}

		m_KnotsS.resize(m_DegreeS + NS + 1);
		for (size_t i = 0; i < m_DegreeS + 1; ++i) {
			m_KnotsS[i] = 0;
			m_KnotsS[mS - i] = 1;
		}
		for (size_t i = 1; i < SS; ++i) {
			m_KnotsS[pS + i] = f64(i) / SS;
		}

	}

	std::tuple<bool, std::vector<Vec4D>&, u64, u64> NURBSSurface::GetControlPoints()
	{
		return std::tuple<bool, std::vector<Vec4D>&, u64, u64>(true,
			                                                   m_ControlPoints,
			                                                   m_tControlPointsCount,
			                                                   m_sControlPointsCount);
	}

	void NURBSSurface::InsertKnot(f64 p_knot, u64 p_times, Direction p_direction)
	{
		if (p_direction == Direction::U) {
			u64 npT = m_tControlPointsCount - 1;
			u64 pT = m_DegreeT;
			u64 mpT = npT + pT + 1;
			u64 r = p_times;
			u64 nqT = npT + r;
			f64 u = p_knot;

			std::vector<f64>& UP = m_KnotsT;
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
			for (size_t i = k + 1; i <= mpT; ++i) UQ[i + r] = UP[i];

			//.........................................
			// Defining s
			u64 s = 0;
			for (size_t i = 0; i < UP.size(); ++i) {
				if (u == UP[i]) {
					s++;
				}
			}

			//.........................................
			// Saving the alphas
			std::vector<f64> alpha((pT - s) * (r+1));
			for (size_t j = 1; j <= r; ++j) {
				u64 L = k - pT + j;
				for (size_t i = 0; i <= pT - j - s; ++i) {
					alpha[i * (r+1) + j] = (u - UP[L + i]) / (UP[i + k + 1] - UP[L + i]);
				}
			}

			
			std::vector<Vec4D>& Pw = m_ControlPoints;
			std::vector<Vec4D> Qw( (nqT + 1) * m_sControlPointsCount );
			std::vector<Vec4D> Rw(pT + 1);
			//.........................................
			// For each row do
			for (size_t row = 0; row < m_sControlPointsCount; ++row) {
				//.........................................
				// Saving unaltered control points
				for (size_t i = 0; i <= k - pT; ++i)  Qw[i * m_sControlPointsCount + row] = Pw[i * m_sControlPointsCount + row];
				for (size_t i = k - s; i <= npT; ++i) Qw[(i + r) * m_sControlPointsCount + row] = Pw[i * m_sControlPointsCount + row];
				//.........................................
				// Loading auxiliary control points
				for (size_t i = 0; i <= pT - s; ++i)  Rw[i] = Pw[(k - pT + i) * m_sControlPointsCount + row];

				//.........................................
				// Inserting the knot r times
				u64 L;
				for (size_t j = 1; j <= r; ++j) {
					L = k - pT + j;
					for (size_t i = 0; i <= pT - j - s; ++i) {
						Rw[i].x = alpha[i * (r+1) + j] * Rw[i + 1].x + (1.0 - alpha[i * (r+1) + j]) * Rw[i].x;
						Rw[i].y = alpha[i * (r+1) + j] * Rw[i + 1].y + (1.0 - alpha[i * (r+1) + j]) * Rw[i].y;
						Rw[i].z = alpha[i * (r+1) + j] * Rw[i + 1].z + (1.0 - alpha[i * (r+1) + j]) * Rw[i].z;
						Rw[i].w = alpha[i * (r+1) + j] * Rw[i + 1].w + (1.0 - alpha[i * (r+1) + j]) * Rw[i].w;
					}
					Qw[L * m_sControlPointsCount + row] = Rw[0];
					Qw[(k + r - j - s) * m_sControlPointsCount + row] = Rw[pT - j - s];
				}

				//.........................................
				// Loading the remaining control points
				for (size_t i = L + 1; i < k - s; ++i) {
					Qw[i * m_sControlPointsCount + row] = Rw[i - L];
				}
				

			}
			m_ControlPoints = std::move(Qw);
			m_KnotsT = std::move(UQ);
			m_tControlPointsCount = nqT + 1;

		} else {
			u64 npS = m_sControlPointsCount - 1;
			u64 pS = m_DegreeS;
			u64 mpS = npS + pS + 1;
			u64 r = p_times;
			u64 nqS = npS + r;
			u64 sNewPointCount = nqS + 1;
			f64 u = p_knot;

			std::vector<f64>& UP = m_KnotsS;
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
			for (size_t i = k + 1; i <= mpS; ++i) UQ[i + r] = UP[i];

			//.........................................
			// Defining s
			u64 s = 0;
			for (size_t i = 0; i < UP.size(); ++i) {
				if (u == UP[i]) {
					s++;
				}
			}

			//.........................................
			// Saving the alphas
			std::vector<f64> alpha((pS - s) * (r + 1));
			for (size_t j = 1; j <= r; ++j) {
				u64 L = k - pS + j;
				for (size_t i = 0; i <= pS - j - s; ++i) {
					alpha[i * (r + 1) + j] = (u - UP[L + i]) / (UP[i + k + 1] - UP[L + i]);
				}
			}

			std::vector<Vec4D>& Pw = m_ControlPoints;
			std::vector<Vec4D> Qw((nqS + 1) * m_tControlPointsCount);
			std::vector<Vec4D> Rw(pS + 1);
			//.........................................
			// For each row do
			for (size_t column = 0; column < m_tControlPointsCount; ++column) {
				//.........................................
				// Saving unaltered control points
				for (size_t i = 0; i <= k - pS; ++i)  Qw[column * sNewPointCount + i] = Pw[column * m_sControlPointsCount + i];
				for (size_t i = k - s; i <= npS; ++i) Qw[column * sNewPointCount + i + r] = Pw[column * m_sControlPointsCount + i];
				//.........................................
				// Loading auxiliary control points
				for (size_t i = 0; i <= pS - s; ++i)  Rw[i] = Pw[column * m_sControlPointsCount + k - pS + i];

				//.........................................
				// Inserting the knot r times
				u64 L;
				for (size_t j = 1; j <= r; ++j) {
					L = k - pS + j;
					for (size_t i = 0; i <= pS - j - s; ++i) {
						Rw[i].x = alpha[i * (r + 1) + j] * Rw[i + 1].x + (1.0 - alpha[i * (r + 1) + j]) * Rw[i].x;
						Rw[i].y = alpha[i * (r + 1) + j] * Rw[i + 1].y + (1.0 - alpha[i * (r + 1) + j]) * Rw[i].y;
						Rw[i].z = alpha[i * (r + 1) + j] * Rw[i + 1].z + (1.0 - alpha[i * (r + 1) + j]) * Rw[i].z;
						Rw[i].w = alpha[i * (r + 1) + j] * Rw[i + 1].w + (1.0 - alpha[i * (r + 1) + j]) * Rw[i].w;
					}
					Qw[column * sNewPointCount + L] = Rw[0];
					Qw[column * sNewPointCount + k + r - j - s] = Rw[pS - j - s];
				}

				//.........................................
				// Loading the remaining control points
				for (size_t i = L + 1; i < k - s; ++i) {
					Qw[column * sNewPointCount + i] = Rw[i - L];
				}


			}
			m_ControlPoints = std::move(Qw);
			m_KnotsS = std::move(UQ);
			m_sControlPointsCount = sNewPointCount;
		}
	}

	void NURBSSurface::DecomposeIntoBezier()
	{
		struct KnotInfo
		{
			f64 value;
			u64 multiplicity;
			u64 k;
		};

		{
			std::vector<KnotInfo> knotInfos;
			f64 currentValue = m_KnotsT[m_KnotsT.size() - 1];
			for (size_t i = 0; i < m_KnotsT.size(); ++i) {
				if (m_KnotsT[i] != currentValue) {
					knotInfos.push_back({ m_KnotsT[i] , 1, i });
					currentValue = m_KnotsT[i];
				}
				else {
					knotInfos[knotInfos.size() - 1].multiplicity++;
				}
			}

			u64 p = m_DegreeT;
			for (size_t i = 1; i < knotInfos.size() - 1; ++i) {
				InsertKnot(knotInfos[i].value, p - knotInfos[i].value, Direction::U);
			}
		}

		{
			std::vector<KnotInfo> knotInfos;
			f64 currentValue = m_KnotsS[m_KnotsS.size() - 1];
			for (size_t i = 0; i < m_KnotsS.size(); ++i) {
				if (m_KnotsS[i] != currentValue) {
					knotInfos.push_back({ m_KnotsS[i] , 1, i });
					currentValue = m_KnotsS[i];
				}
				else {
					knotInfos[knotInfos.size() - 1].multiplicity++;
				}
			}

			u64 p = m_DegreeS;
			for (size_t i = 1; i < knotInfos.size() - 1; ++i) {
				InsertKnot(knotInfos[i].value, p - knotInfos[i].value, Direction::V);
			}
		}
	}

}