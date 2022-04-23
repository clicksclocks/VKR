#include "pchheader.h"
#include "Geo.h"

#include "Curves/NURBSCurve.h"
#include "Surfaces/NURBSSurface.h"

#include "Curves/BezierCurve.h"
#include "Surfaces/BezierSurface.h"

namespace Horyzen::Geo {

	void Geo::AddSurface()
	{
		GObject* gObject = new GObject(SurfaceType::NURBS, &m_GPointCollection);

		m_GeometryObjects.push_back(gObject);

	}

	void Geo::AddCurve()
	{
		GObject* gObject = new GObject(CurveType::NURBS, &m_GPointCollection);

		m_GeometryObjects.push_back(gObject);
	}

	void Geo::AddCurve(std::shared_ptr<NURBSCurve> p_curve)
	{
		GObject* gObject = new GObject(CurveType::NURBS, p_curve, &m_GPointCollection);

		m_GeometryObjects.push_back(gObject);
	}

	void Geo::AddSurface(std::shared_ptr<NURBSSurface> p_surface)
	{
		GObject* gObject = new GObject(SurfaceType::NURBS, p_surface, &m_GPointCollection);

		m_GeometryObjects.push_back(gObject);
	}

	void Geo::AddCurve(std::shared_ptr<BezierCurve> p_curve)
	{
		GObject* gObject = new GObject(CurveType::Bezier, p_curve, &m_GPointCollection);

		m_GeometryObjects.push_back(gObject);
	}

	void Geo::AddSurface(std::shared_ptr<BezierSurface> p_surface)
	{
		GObject* gObject = new GObject(SurfaceType::Bezier, p_surface, &m_GPointCollection);

		m_GeometryObjects.push_back(gObject);
	}

	void Geo::AddDebugSurface(const std::vector<Vec4D>& p_points, u64 p_tPointCount, u64 p_sPointCount, u64 p_maxLevels, u64 p_multiplier)
	{
		DebugSurface* debugSurface = new DebugSurface(p_points, p_tPointCount, p_sPointCount, p_maxLevels, p_multiplier);

		m_DebugObjects.push_back(debugSurface);
	}

	void Geo::ClearDebugObjectStorage()
	{
		for (auto dObj : m_DebugObjects) {
			delete dObj;
		}
		m_DebugObjects.clear();
	}



}