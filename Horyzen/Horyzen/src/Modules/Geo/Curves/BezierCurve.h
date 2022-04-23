#pragma once
#include "AbstractCurve.h"

namespace Horyzen::Geo {

	class HORYZEN_OBJECT BezierCurve : public AbstractCurve
	{
	public:

		friend class BezierCurveBuilder;
		friend class BezierCurveProjector;

		BezierCurve();
		BezierCurve(const std::vector<Vec4D>& p_controlPoints,
					const f64                 p_tMin,
					const f64                 p_tMax);

		virtual std::pair<bool, std::vector<Vec4D>&> GetControlPoints() override;

	private:

		std::vector<Vec4D> m_ControlPoints;

	};

}

