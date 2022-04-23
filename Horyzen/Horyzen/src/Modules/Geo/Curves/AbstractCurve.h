#pragma once

#include "Horyzen/Core.h"
#include "Horyzen/Math/Vec4.h"

#include <utility>

namespace Horyzen::Geo {

	class HORYZEN_OBJECT AbstractCurve
	{
	public:

		friend class AbstractCurveBuilder;

		AbstractCurve(f64 p_tMin,  // Минимальное значение параметра t
			          f64 p_tMax)  // Максимальное значение параметра t
			: m_tMin(p_tMin),
			  m_tMax(p_tMax)
		{}

		virtual ~AbstractCurve() = default;

		virtual std::pair<bool, std::vector<Vec4D>&> GetControlPoints() = 0;

		inline const f64 GetTmin() const { return m_tMin; }
		inline const f64 GetTmax() const { return m_tMax; }

	protected:

		f64 m_tMin, m_tMax;

	};

}



