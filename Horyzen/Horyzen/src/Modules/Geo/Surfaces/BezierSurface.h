#pragma once
#include "AbstractSurface.h"

namespace Horyzen::Geo {

	class HORYZEN_OBJECT BezierSurface : public AbstractSurface
	{
	public:

		friend class BezierSurfaceBuilder;

		BezierSurface();
			

		virtual ~BezierSurface() = default;

		virtual std::tuple<bool, std::vector<Vec4D>&, u64, u64> GetControlPoints() override;

	private:

		std::vector<Vec4D> m_ControlPoints;

		u64 m_tControlPointsCount{ 0 };
		u64 m_sControlPointsCount{ 0 };

	};

}

