#pragma once
#include "AbstractSurface.h"

namespace Horyzen::Geo {

	class HORYZEN_OBJECT NURBSSurface : public AbstractSurface
	{
	public:

		enum class Direction {
			U, V
		};

		friend class NURBSSurfaceBuilder;
		friend class NURBSSurfaceBuilder2;

		NURBSSurface();

		virtual std::tuple<bool, std::vector<Vec4D>&, u64, u64> GetControlPoints() override;

		void InsertKnot(f64 p_knot, u64 p_times, Direction p_direction);
		void DecomposeIntoBezier();

	private:

		std::vector<f64> m_KnotsT;
		std::vector<f64> m_KnotsS;
		u64 m_DegreeT;
		u64 m_DegreeS;
		std::vector<Vec4D> m_ControlPoints;

		u64 m_tControlPointsCount{ 0 };
		u64 m_sControlPointsCount{ 0 };
		
	};

}

