#pragma once
#include "AbstractSurfaceBuilder.h"

#include "../Surfaces/NURBSSurface.h"

namespace Horyzen::Geo {

	class NURBSSurfaceBuilder : public AbstractSurfaceBuilder
	{
	public:

		NURBSSurfaceBuilder(std::shared_ptr<NURBSSurface> p_surfaceObject);
		virtual ~NURBSSurfaceBuilder() = default;

		virtual Vec3D CalculatePoint(f64 p_t, f64 p_s)  override;
		virtual std::pair<Vec3D, Vec3D> CalculateFirstDerivative(f64 p_t, f64 p_s)  override;
		virtual std::pair<Vec3D, Vec3D> CalculateSecondDerivative(f64 p_t, f64 p_s)  override { return std::make_pair(Vec3D(), Vec3D()); }
		virtual std::vector<Vec3D> CalculateSpan(f64 p_tMin,
			                                     f64 p_tMax,
			                                     f64 p_sMin,
			                                     f64 p_sMax,
			                                     u64 p_tSubdivisions,
		                                         u64 p_sSubdivisions) override { return std::vector<Vec3D>(); }

	private:

		std::pair<u64, u64> FindSpan(f64 p_t, f64 p_s);
		void BasisFuns(u64 p_i, f64 p_t, u64 p_j, f64 p_s);

		void BasisFunsFirstDerivatives(u64 p_i, f64 p_t, u64 p_j, f64 p_s);

	private:

		std::shared_ptr<NURBSSurface> m_Surface;

		std::vector<f64> m_NT;
		std::vector<f64> m_NS;

		std::vector<f64> m_LeftT;
		std::vector<f64> m_RightT;

		std::vector<f64> m_LeftS;
		std::vector<f64> m_RightS;

		std::vector<f64> m_DersS;
		std::vector<f64> m_DersT;

		std::vector<f64> m_nduS;
		std::vector<f64> m_aS;

		std::vector<f64> m_nduT;
		std::vector<f64> m_aT;
		
	};

}

