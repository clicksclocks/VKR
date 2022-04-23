#pragma once
#include "Horyzen/Math/Vec3.h"

#include "../Surfaces/AbstractSurface.h"

#include <memory>


namespace Horyzen::Geo {

	class AbstractSurfaceBuilder
	{
	public:

		inline AbstractSurfaceBuilder(std::shared_ptr<AbstractSurface> p_surfaceObject)
			: m_SurfaceObject(p_surfaceObject)
		{}
		virtual ~AbstractSurfaceBuilder() = default;

		virtual Vec3D CalculatePoint(f64 p_t, f64 p_s) = 0;
		virtual std::pair<Vec3D, Vec3D> CalculateFirstDerivative(f64 p_t, f64 p_s) = 0;
		virtual std::pair<Vec3D, Vec3D> CalculateSecondDerivative(f64 p_t, f64 p_s) = 0;
		virtual std::vector<Vec3D> CalculateSpan(f64 p_tMin, 
			                                     f64 p_tMax,
			                                     f64 p_sMin,
			                                     f64 p_sMax,
			                                     u64 p_tSubdivisions,
			                                     u64 p_sSubdivisions) = 0;

	private:

		std::shared_ptr<AbstractSurface> m_SurfaceObject;

	};


}

