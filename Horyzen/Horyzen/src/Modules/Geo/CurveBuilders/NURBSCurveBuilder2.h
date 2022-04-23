#pragma once

#include "AbstractCurveBuilder.h"
#include "../Curves/NURBSCurve.h"

namespace Horyzen::Geo {

	class NURBSCurveBuilder2 : public AbstractCurveBuilder
	{
	public:

		NURBSCurveBuilder2(std::shared_ptr<NURBSCurve> p_curveObject);
		virtual ~NURBSCurveBuilder2() = default;

		virtual Vec3D CalculatePoint(f64 p_t) override;
		virtual Vec3D CalculateFirstDerivative(f64 p_t) override;
		virtual Vec3D CalculateSecondDerivative(f64 p_t) override { return Vec3D(); }
		virtual std::vector<Vec3D> CalculateSpan(f64 p_tMin, f64 p_tMax, u64 p_subdivisions) override { return std::vector<Vec3D>(); }


	private:

		u64 FindSpan(f64 p_t);
	//	void BasisFuns(u64 p_i, f64 p_t);

	private:

		std::shared_ptr<NURBSCurve> m_Curve;
		std::vector<f64> m_N;
		std::vector<f64> m_Left;
		std::vector<f64> m_Right;

		std::vector<Vec3D> m_PointBuffer;
		std::vector<f64> m_WeightBuffer;

	};

}

