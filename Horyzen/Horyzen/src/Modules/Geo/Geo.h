#pragma once

#include "GeoTypes.h"
#include "GPointCollection.h"
#include "GObject.h"
#include "DebugTools/DebugSurface.h"
#include "Horyzen/EntityComponentSystem/Component.h"



namespace Horyzen::Geo {

	class NURBSCurve;
	class NURBSSurface;
	class BezierCurve;
	class BezierSurface;

	class HORYZEN_OBJECT Geo : public ECS::Component<Geo>
	{
	public:

		friend class GeoRenderSystem;
		friend class GPointPickingSystem;

		void AddCurve();
		void AddSurface();

		void AddCurve(std::shared_ptr<NURBSCurve> p_curve);
		void AddSurface(std::shared_ptr<NURBSSurface> p_surface);

		void AddCurve(std::shared_ptr<BezierCurve> p_curve);
		void AddSurface(std::shared_ptr<BezierSurface> p_surface);

		void AddDebugSurface(const std::vector<Vec4D>& p_points, u64 p_tPointCount, u64 p_sPointCount, u64 p_maxLevels, u64 p_multiplier);

		void ClearDebugObjectStorage();

	private:

		GPointCollection m_GPointCollection;
		std::vector<GObject*> m_GeometryObjects;
		std::vector<DebugSurface*> m_DebugObjects;

	};

}

