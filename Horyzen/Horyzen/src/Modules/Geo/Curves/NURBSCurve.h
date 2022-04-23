#pragma once
#include "AbstractCurve.h"


namespace Horyzen::Geo {

	class HORYZEN_OBJECT NURBSCurve : public AbstractCurve
	{
	public:

		friend class NURBSCurveBuilder;
		friend class NURBSCurveBuilder2;

		NURBSCurve();
		NURBSCurve(const std::vector<Vec4D>& p_controlPoints,
			       const std::vector<f64>&   p_knots,
			       const u64                 p_degree,
			       const f64                 p_tMin,
			       const f64                 p_tMax);

		virtual std::pair<bool, std::vector<Vec4D>&> GetControlPoints() override;

		void InsertKnot(f64 p_knot, u64 p_times);
		void DecomposeIntoBezier();

		std::vector<std::vector<Vec4D>> GetBezierControlPoints();

	private:
	
		std::vector<Vec4D> m_ControlPoints;
		std::vector<f64>   m_Knots;
		u64 m_Degree;
		
	};

}
