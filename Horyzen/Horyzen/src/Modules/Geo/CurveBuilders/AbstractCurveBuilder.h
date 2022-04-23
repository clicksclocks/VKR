#pragma once

#include "Horyzen/Math/Vec3.h"
#include "../Curves/AbstractCurve.h"

#include <memory>

namespace Horyzen::Geo {

	class AbstractCurve;
	class AbstractCurveBuilder
	{
	public:

		inline AbstractCurveBuilder(std::shared_ptr<AbstractCurve> p_curveObject)
			: m_CurveObject(p_curveObject),
			  m_tMin(p_curveObject->m_tMin),
			  m_tMax(p_curveObject->m_tMax)
		{}
		virtual ~AbstractCurveBuilder() = default;
		
		virtual Vec3D CalculatePoint(f64 p_t) = 0;
		virtual Vec3D CalculateFirstDerivative(f64 p_t) = 0;
		virtual Vec3D CalculateSecondDerivative(f64 p_t) = 0;
		virtual std::vector<Vec3D> CalculateSpan(f64 p_tMin, f64 p_tMax, u64 p_subdivisions) = 0;

	private:

		std::shared_ptr<AbstractCurve> m_CurveObject;
		f64 m_tMin, m_tMax;

	};

}


