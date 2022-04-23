#pragma once

#include "AbstractCurveBuilder.h"
#include "../Curves/BezierCurve.h"

namespace Horyzen::Geo {

	class BezierCurveBuilder : public AbstractCurveBuilder
	{
	public:

		BezierCurveBuilder(std::shared_ptr<BezierCurve> p_curveObject);
		virtual ~BezierCurveBuilder() = default;

		virtual Vec3D CalculatePoint(f64 p_t) override;
		virtual Vec3D CalculateFirstDerivative(f64 p_t) override { return Vec3D(); }
		virtual Vec3D CalculateSecondDerivative(f64 p_t) override { return Vec3D(); }
		virtual std::vector<Vec3D> CalculateSpan(f64 p_tMin, f64 p_tMax, u64 p_subdivisions) override { return std::vector<Vec3D>(); }

	private:

		f64 Bernstein(u64 i, f64 t);
		void AllBernstein(f64 t);

	
	private:
		
		std::shared_ptr<BezierCurve> m_Curve;

		u64 n;
		std::vector<f64> m_Temp;
		std::vector<f64> m_BersteinValues;

	};

}



