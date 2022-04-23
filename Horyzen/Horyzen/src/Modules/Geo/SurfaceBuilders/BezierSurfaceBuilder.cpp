#include "pchheader.h"
#include "BezierSurfaceBuilder.h"

#include "../Surfaces/BezierSurface.h"


namespace Horyzen::Geo {



	BezierSurfaceBuilder::BezierSurfaceBuilder(std::shared_ptr<BezierSurface> p_surfaceObject)
		: AbstractSurfaceBuilder(p_surfaceObject),
		m_Surface(p_surfaceObject),
		m_BersteinValuesT(p_surfaceObject->m_tControlPointsCount),
		m_BersteinValuesS(p_surfaceObject->m_sControlPointsCount)
	{}

	Vec3D BezierSurfaceBuilder::CalculatePoint(f64 p_t, f64 p_s)
	{
		Vec3D o_point;
		AllBernstein(p_t, p_s);

		u64 n = m_Surface->m_tControlPointsCount - 1;
		u64 m = m_Surface->m_sControlPointsCount - 1;

		auto& P = std::get<1>(m_Surface->GetControlPoints());
		for (size_t i = 0; i <= n; ++i) {
			for (size_t j = 0; j <= m; ++j) {
				o_point.x += P[i*(m+1) + j].x * m_BersteinValuesT[i] * m_BersteinValuesS[j];
				o_point.y += P[i*(m+1) + j].y * m_BersteinValuesT[i] * m_BersteinValuesS[j];
				o_point.z += P[i*(m+1) + j].z * m_BersteinValuesT[i] * m_BersteinValuesS[j];
			}
			
		}
		return o_point;
	}

	void BezierSurfaceBuilder::AllBernstein(f64 p_t, f64 p_s)
	{
		u64 n = m_Surface->m_tControlPointsCount - 1;
		u64 m = m_Surface->m_sControlPointsCount - 1;

		m_BersteinValuesT[0] = 1.0;
		f64 t1 = 1.0 - p_t;
		for (size_t j = 1; j <= n; ++j) {
			f64 saved = 0;
			for (size_t k = 0; k < j; ++k) {
				f64 temp = m_BersteinValuesT[k];
				m_BersteinValuesT[k] = saved + t1 * temp;
				saved = p_t * temp;
			}
			m_BersteinValuesT[j] = saved;
		}

		m_BersteinValuesS[0] = 1.0;
		f64 s1 = 1.0 - p_s;
		for (size_t j = 1; j <= m; ++j) {
			f64 saved = 0;
			for (size_t k = 0; k < j; ++k) {
				f64 temp = m_BersteinValuesS[k];
				m_BersteinValuesS[k] = saved + s1 * temp;
				saved = p_s * temp;
			}
			m_BersteinValuesS[j] = saved;
		}
	}

}