#pragma once

#include "Horyzen/Core.h"
#include "Horyzen/Math/Vec4.h"

namespace Horyzen::Geo {

	class HORYZEN_OBJECT AbstractSurface
	{
	public:

		AbstractSurface(f64 p_tMin,  // Минимальное значение параметра t
			            f64 p_tMax,  // Максимальное значение параметра t
			            f64 p_sMin,  // Минимальное значение параметра s
			            f64 p_sMax)  // Максимальное значение параметра s
			: m_tMin(p_tMin),
			  m_tMax(p_tMax),
			  m_sMin(p_sMin),
			  m_sMax(p_sMax)
		{}

		virtual ~AbstractSurface() = default;

		virtual std::tuple<bool, std::vector<Vec4D>&, u64, u64> GetControlPoints() = 0;

		inline const f64 GetTmin() const { return m_tMin; }
		inline const f64 GetTmax() const { return m_tMax; }
		inline const f64 GetSmin() const { return m_sMin; }
		inline const f64 GetSmax() const { return m_sMax; }

	private:

		f64 m_tMin, m_tMax;
		f64 m_sMin, m_sMax;

	};

}

