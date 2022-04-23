#pragma once
#include "AbstractSurfaceBuilder.h"

#include "../Surfaces/BezierSurface.h"

namespace Horyzen::Geo {

	class BezierSurfaceBuilder : public AbstractSurfaceBuilder
	{
	public:

		BezierSurfaceBuilder(std::shared_ptr<BezierSurface> p_surfaceObject);
		virtual ~BezierSurfaceBuilder() = default;

		virtual Vec3D CalculatePoint(f64 p_t, f64 p_s)  override;
		virtual std::pair<Vec3D, Vec3D> CalculateFirstDerivative(f64 p_t, f64 p_s)  override { return std::make_pair(Vec3D(), Vec3D()); }
		virtual std::pair<Vec3D, Vec3D> CalculateSecondDerivative(f64 p_t, f64 p_s)  override { return std::make_pair(Vec3D(), Vec3D()); }
		virtual std::vector<Vec3D> CalculateSpan(f64 p_tMin,
			                                     f64 p_tMax,
			                                     f64 p_sMin,
			                                     f64 p_sMax,
			                                     u64 p_tSubdivisions,
			                                     u64 p_sSubdivisions) override { return std::vector<Vec3D>(); }

	private:

		void AllBernstein(f64 p_t, f64 p_s);

	private:

		std::shared_ptr<BezierSurface> m_Surface;

		std::vector<f64> m_BersteinValuesT;
		std::vector<f64> m_BersteinValuesS;

	};

}
