#pragma once

#include "GeoTypes.h"
#include "GPoint.h"

namespace Horyzen
{
	class VertexArray;
	class IndexBuffer;
}


namespace Horyzen::Geo {
	
	class AbstractSurface;
	class AbstractCurve;
	class AbstractSurfaceBuilder;
	class AbstractCurveBuilder;
	class GPointCollection;
	class GObject
	{
	public:

		friend class GeoRenderSystem;

		GObject(CurveType p_type, GPointCollection* p_collection);
		GObject(CurveType p_type, std::shared_ptr<AbstractCurve> p_curve, GPointCollection* p_collection);

		GObject(SurfaceType p_type, GPointCollection* p_collection);
		GObject(SurfaceType p_type, std::shared_ptr<AbstractSurface> p_surface, GPointCollection* p_collection);

		std::vector<GPoint*> m_GPointReferences;

		void Draw() const;
		void DrawDerivative() const;
		void DrawBrokenLine() const;
		void Update();
		void CreateGPointsForGeometry();

	private:

		void InitCurveMode();
		void InitSurfaceMode();

		void SetUpCurve(CurveType p_type, std::shared_ptr<AbstractCurve> p_curve = nullptr);
		void SetUpSurface(SurfaceType p_type, std::shared_ptr<AbstractSurface> p_surface = nullptr);

		void BuildBuffers();
		void BuildBuffersCurve();
		void BuildBuffersSurface();

		void UpdateSurface();
		void UpdateCurve();

		void DrawCurve() const;
		void DrawSurface() const;

		void BuildBrokenLineIndexBufferForCurve();
		void BuildBrokenLineIndexBufferForSurface();

		void CreateGPoints(std::vector<Vec4D>& p_controlPoints);

	private:

		enum class Mode
		{
			Surface,
			Curve
		};

		struct CurveInfoData
		{
			std::shared_ptr<AbstractCurve> Curve{ nullptr };
			std::unique_ptr<AbstractCurveBuilder> CurveBuilder{ nullptr };
			// Visual data
			u64 Subdivisions{ 40000 };
			u64 SubdivisionsPrev{ 0 };

			bool ShowDerivatives = false;
		};

		struct SurfaceInfoData
		{
			std::shared_ptr<AbstractSurface> Surface{ nullptr };
			std::unique_ptr<AbstractSurfaceBuilder> SurfaceBuilder{ nullptr };
			// Visual data
			u64 tSubdivisions{ 200 };
			u64 sSubdivisions{ 200 };

			u64 tSubdivisionsPrev{ 0 };
			u64 sSubdivisionsPrev{ 0 };

			bool ShowDerivatives = false;

			SurfaceDrawMode DrawMode{ SurfaceDrawMode::Grid };
			std::vector<Vec3F> Normals;
		};

		std::unique_ptr<CurveInfoData> m_CurveInfo;
		std::unique_ptr<SurfaceInfoData> m_SurfaceInfo;

		Mode m_Mode;
		GPointCollection* m_GPointCollectionRef;

		std::vector<u64> m_GPointIDs;

		// Visual data
		Color::RGBu m_Color{ 255, 0, 0 };
		std::shared_ptr<VertexArray> m_VertexArray;
		std::vector<Vec3F> m_Buffer;

		// Derivatives
		Color::RGBu m_DerivativeColor{ 60, 100, 0 };
		std::shared_ptr<VertexArray> m_DerivativeVertexArray;
		std::vector<Vec3F> m_DerivativeBuffer;

		// Broken lines
		bool m_DrawBrokenLines{ true };
		std::shared_ptr<IndexBuffer> m_BrokenLineIndexBuffer;
		Color::RGBu m_BrokenLineColor{ 0, 100, 230 };

	};

}



