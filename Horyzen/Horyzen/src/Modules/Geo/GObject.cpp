#include "pchheader.h"
#include "GObject.h"

#include "Surfaces/AbstractSurface.h"
#include "Curves/AbstractCurve.h"
#include "GPointCollection.h"
#include "Horyzen/Renderer/VertexArray.h"
#include "SurfaceBuilders/AbstractSurfaceBuilder.h"
#include "Horyzen/Renderer/Renderer.h"


//........................................
// Curves
#include "Curves/BezierCurve.h"
#include "Curves/NURBSCurve.h"

//........................................
// Curves builders
#include "CurveBuilders/BezierCurveBuilder.h"
#include "CurveBuilders/NURBSCurveBuilder.h"
#include "CurveBuilders/NURBSCurveBuilder2.h"

//........................................
// Surfaces
#include "Surfaces/BezierSurface.h"
#include "Surfaces/NURBSSurface.h"

//........................................
// Surface builders
#include "SurfaceBuilders/BezierSurfaceBuilder.h"
#include "SurfaceBuilders/NURBSSurfaceBuilder.h"
#include "SurfaceBuilders/NURBSSurfaceBuilder2.h"

#include "Horyzen/Profiler/Profiler.h"


namespace Horyzen::Geo {

	GObject::GObject(CurveType p_type, GPointCollection* p_collection)
		: m_Mode(Mode::Curve),
		  m_GPointCollectionRef(p_collection)
	{
		InitCurveMode();
		SetUpCurve(p_type);
		CreateGPointsForGeometry();
		BuildBrokenLineIndexBufferForCurve();
	}

	GObject::GObject(CurveType p_type, std::shared_ptr<AbstractCurve> p_curve, GPointCollection* p_collection)
		: m_Mode(Mode::Curve),
		  m_GPointCollectionRef(p_collection)
	{
		InitCurveMode();
		SetUpCurve(p_type, p_curve);
		CreateGPointsForGeometry();
		BuildBrokenLineIndexBufferForCurve();
	}

	GObject::GObject(SurfaceType p_type, GPointCollection* p_collection)
		: m_Mode(Mode::Surface),
		  m_GPointCollectionRef(p_collection)
	{
		InitSurfaceMode();
		SetUpSurface(p_type);
		CreateGPointsForGeometry();
		BuildBrokenLineIndexBufferForSurface();
	}

	GObject::GObject(SurfaceType p_type, std::shared_ptr<AbstractSurface> p_surface, GPointCollection* p_collection)
		: m_Mode(Mode::Surface),
		  m_GPointCollectionRef(p_collection)
	{
		InitSurfaceMode();
		SetUpSurface(p_type, p_surface);
		CreateGPointsForGeometry();
		BuildBrokenLineIndexBufferForSurface();
	}

	void GObject::Draw() const
	{
		switch (m_Mode) {
			case Mode::Curve:   DrawCurve();   return;
			case Mode::Surface: DrawSurface(); return;
		}
	}

	void GObject::DrawDerivative() const
	{
		Renderer::Submit(m_DerivativeVertexArray, BasePrimitive::Lines);
	}

	void GObject::DrawCurve() const
	{
		Renderer::Submit(m_VertexArray, BasePrimitive::LineStrip);
	}

	void GObject::DrawSurface() const
	{
		Renderer::Submit(m_VertexArray, BasePrimitive::LineStrip);
	}

	void GObject::DrawBrokenLine() const
	{
		auto tmpIndexBuffer = m_GPointCollectionRef->m_VertexArray->GetIndexBuffer();
		m_GPointCollectionRef->m_VertexArray->SetIndexBuffer(m_BrokenLineIndexBuffer);
		Renderer::Submit(m_GPointCollectionRef->m_VertexArray, BasePrimitive::LineStrip);
		m_GPointCollectionRef->m_VertexArray->SetIndexBuffer(tmpIndexBuffer);
	}

	
	void GObject::Update()
	{
		switch (m_Mode) {
			case Mode::Curve:   UpdateCurve();   return;
			case Mode::Surface: UpdateSurface(); return;
		}
	}

	void GObject::CreateGPointsForGeometry()
	{
		switch (m_Mode) {
			case Mode::Surface:
			{
				auto controlPoints = m_SurfaceInfo->Surface->GetControlPoints();
				if (std::get<0>(controlPoints)) {
					CreateGPoints(std::get<1>(controlPoints));
				}
			} break;

			case Mode::Curve:
			{
				auto controlPoints = m_CurveInfo->Curve->GetControlPoints();
				if (controlPoints.first) {
					CreateGPoints(controlPoints.second);
				}
			}
		}

	}

	void GObject::InitCurveMode()
	{
		m_CurveInfo = std::make_unique<CurveInfoData>();
	}

	void GObject::InitSurfaceMode()
	{
		m_SurfaceInfo = std::make_unique<SurfaceInfoData>();
	}

	void GObject::SetUpCurve(CurveType p_type, std::shared_ptr<AbstractCurve> p_curve)
	{
		switch (p_type) {
			case CurveType::Bezier: 
			{
				if (!p_curve) m_CurveInfo->Curve = std::make_shared<BezierCurve>();
				else          m_CurveInfo->Curve = p_curve;

				m_CurveInfo->CurveBuilder = std::make_unique<BezierCurveBuilder>(
					std::dynamic_pointer_cast<BezierCurve>(m_CurveInfo->Curve));
			} break;

			case CurveType::NURBS:
			{
				if (!p_curve) m_CurveInfo->Curve = std::make_shared<NURBSCurve>();
				else          m_CurveInfo->Curve = p_curve;

				m_CurveInfo->CurveBuilder = std::make_unique<NURBSCurveBuilder2>(
					std::dynamic_pointer_cast<NURBSCurve>(m_CurveInfo->Curve));
			} break;
		}
		Update();
	}

	void GObject::SetUpSurface(SurfaceType p_type, std::shared_ptr<AbstractSurface> p_surface)
	{
		switch (p_type) {
			case SurfaceType::Bezier: {
				if (!p_surface) m_SurfaceInfo->Surface = std::make_shared<BezierSurface>();
				else            m_SurfaceInfo->Surface = p_surface;

				m_SurfaceInfo->SurfaceBuilder = std::make_unique<BezierSurfaceBuilder>(
					std::dynamic_pointer_cast<BezierSurface>(m_SurfaceInfo->Surface));
			} break;

			case SurfaceType::NURBS: {
				if (!p_surface) m_SurfaceInfo->Surface = std::make_shared<NURBSSurface>();
				else            m_SurfaceInfo->Surface = p_surface;

				m_SurfaceInfo->SurfaceBuilder = std::make_unique<NURBSSurfaceBuilder2>(
					std::dynamic_pointer_cast<NURBSSurface>(m_SurfaceInfo->Surface));
			} break;
		}
		Update();
	}

	void GObject::BuildBuffers()
	{
		m_VertexArray.reset();
		m_VertexArray = VertexArray::Create();
		m_DerivativeVertexArray.reset();
		m_DerivativeVertexArray = VertexArray::Create();
		switch (m_Mode) {
			case Mode::Curve:   BuildBuffersCurve();   return;
			case Mode::Surface: BuildBuffersSurface(); return;
		}
	}

	void GObject::BuildBuffersCurve()
	{
		//..................................................................
		// Building buffer for points
		{
			u64 byteSize = (m_CurveInfo->Subdivisions + 1) * 3 * sizeof(f32);
			auto vertexBuffer = VertexBuffer::Create(byteSize);
			VertexBufferLayout layout =
			{
				{ ShaderDataType::Float3, "a_Position" }
			};
			vertexBuffer->SetLayout(layout);

			std::vector<u32> indices(m_CurveInfo->Subdivisions + 1);
			for (size_t i = 0; i < indices.size(); ++i) {
				indices[i] = i;
			}
			auto indexBuffer = IndexBuffer::Create(indices.data(), indices.size());

			m_VertexArray->AddVertexBuffer(vertexBuffer);
			m_VertexArray->SetIndexBuffer(indexBuffer);
			m_Buffer.resize(m_CurveInfo->Subdivisions + 1);
		}
		
		//..................................................................
		// Building buffer for derivatives
		{
			u64 byteSize = (m_CurveInfo->Subdivisions + 1) * 3 * sizeof(f32) * 2;
			auto vertexBuffer = VertexBuffer::Create(byteSize);
			VertexBufferLayout layout =
			{
				{ ShaderDataType::Float3, "a_Position" }
			};
			vertexBuffer->SetLayout(layout);

			std::vector<u32> indices((m_CurveInfo->Subdivisions + 1) * 2);
			for (size_t i = 0; i < indices.size(); ++i) {
				indices[i] = i;
			}
			auto indexBuffer = IndexBuffer::Create(indices.data(), indices.size());

			m_DerivativeVertexArray->AddVertexBuffer(vertexBuffer);
			m_DerivativeVertexArray->SetIndexBuffer(indexBuffer);
			m_DerivativeBuffer.resize((m_CurveInfo->Subdivisions + 1)*2);
		}
	}

	void GObject::BuildBuffersSurface()
	{
		u64 tPoints = m_SurfaceInfo->tSubdivisions + 1;
		u64 sPoints = m_SurfaceInfo->sSubdivisions + 1;

		//..................................................................
		// Building buffer for points
		{
			u64 byteSize = tPoints * sPoints * 3 * sizeof(f32);
			auto vertexBuffer = VertexBuffer::Create(byteSize);
			VertexBufferLayout layout =
			{
				{ ShaderDataType::Float3, "a_Position" }
			};
			vertexBuffer->SetLayout(layout);


			bool forward = true;
			std::vector<u32> indices;
			for (size_t i = 0; i < tPoints; ++i) {
				if (forward) {
					for (size_t j = 0; j < sPoints; ++j) {
						indices.push_back(i * sPoints + j);
					}
				}
				else {
					for (i64 j = sPoints - 1; j >= 0; --j) {
						indices.push_back(i * sPoints + j);
					}
				}
				forward = !forward;
			}
			if (forward) {
				forward = false;
				for (size_t j = 0; j < sPoints; ++j) {
					if (forward) {
						for (size_t i = 0; i < tPoints; ++i) {
							indices.push_back(i * sPoints + j);
						}
					}
					else {
						for (i64 i = tPoints - 1; i >= 0; --i) {
							indices.push_back(i * sPoints + j);
						}
					}
					forward = !forward;
				}

			}
			else {
				for (i64 j = sPoints - 1; j >= 0; --j) {
					if (forward) {
						for (size_t i = 0; i < tPoints; ++i) {
							indices.push_back(i * sPoints + j);
						}
					}
					else {
						for (i64 i = tPoints - 1; i >= 0; --i) {
							indices.push_back(i * sPoints + j);
						}
					}
					forward = !forward;
				}

			}
			auto indexBuffer = IndexBuffer::Create(indices.data(), indices.size());

			m_VertexArray->AddVertexBuffer(vertexBuffer);
			m_VertexArray->SetIndexBuffer(indexBuffer);
			m_Buffer.resize(tPoints * sPoints);
		}
		//..................................................................
		// Building buffer for derivatives
		{
			u64 byteSize = tPoints * sPoints * 3 * sizeof(f32) * 2;
			auto vertexBuffer = VertexBuffer::Create(byteSize);
			VertexBufferLayout layout =
			{
				{ ShaderDataType::Float3, "a_Position" }
			};
			vertexBuffer->SetLayout(layout);

			std::vector<u32> indices;
			for (size_t i = 0; i < tPoints; ++i) {
				for (size_t j = 0; j < sPoints; ++j) {
					indices.push_back((i * sPoints + j) * 2);
					indices.push_back((i * sPoints + j) * 2 + 1);
				}
			}
			
			auto indexBuffer = IndexBuffer::Create(indices.data(), indices.size());

			m_DerivativeVertexArray->AddVertexBuffer(vertexBuffer);
			m_DerivativeVertexArray->SetIndexBuffer(indexBuffer);
			m_DerivativeBuffer.resize(tPoints * sPoints * 2);
		}

	}


	void GObject::UpdateCurve()
	{
		
		if (m_CurveInfo->Subdivisions != m_CurveInfo->SubdivisionsPrev) {
			BuildBuffers();
			m_CurveInfo->SubdivisionsPrev = m_CurveInfo->Subdivisions;
		}
		f64 tMin = m_CurveInfo->Curve->GetTmin();
		f64 spanLenght = m_CurveInfo->Curve->GetTmax() - m_CurveInfo->Curve->GetTmin();

		//f64 tMin = -100.0;
		//f64 spanLenght = 200.0;
		
		{
			HORYZEN_PROFILE_FUNCTION();
			{
				f64 step = spanLenght / m_CurveInfo->Subdivisions;
				u64 steps = m_CurveInfo->Subdivisions + 1;
				for (size_t i = 0; i < steps; ++i) {
					f64 t = tMin + i * step;
					Vec3D position = m_CurveInfo->CurveBuilder->CalculatePoint(t);
					m_Buffer[i] = {
						static_cast<f32>(position.x),
						static_cast<f32>(position.y),
						static_cast<f32>(position.z)
					};
				}
			}
		}

	//	m_CurveInfo->ShowDerivatives = true;
		if (m_CurveInfo->ShowDerivatives) {

			f64 step = spanLenght / m_CurveInfo->Subdivisions;
			u64 steps = m_CurveInfo->Subdivisions + 1;
			for (size_t i = 0; i < steps; ++i) {
				f64 t = tMin + i * step;
				Vec3D derivative = (m_CurveInfo->CurveBuilder->CalculateFirstDerivative(t)).scaled(0.2);
				m_DerivativeBuffer[2*i] = m_Buffer[i];
				m_DerivativeBuffer[2*i + 1] = m_Buffer[i] + Vec3F(static_cast<f32>(derivative.x),
		                                                          static_cast<f32>(derivative.y),
					                                              static_cast<f32>(derivative.z));
			}
		}

		u64 byteSize = (m_CurveInfo->Subdivisions + 1) * 3 * sizeof(f32);
		m_VertexArray->GetVertexBuffers()[0]->UpdateSubData(0, byteSize, m_Buffer.data());

		if (m_CurveInfo->ShowDerivatives) {
			u64 derivativeByteSize = (m_CurveInfo->Subdivisions + 1) * 3 * sizeof(f32) * 2;
			m_DerivativeVertexArray->GetVertexBuffers()[0]->UpdateSubData(0, derivativeByteSize, m_DerivativeBuffer.data());
		}
	}

	void GObject::UpdateSurface()
	{
		if (m_SurfaceInfo->tSubdivisions != m_SurfaceInfo->tSubdivisionsPrev ||
			m_SurfaceInfo->sSubdivisions != m_SurfaceInfo->sSubdivisionsPrev) {
			BuildBuffers();
			m_SurfaceInfo->tSubdivisionsPrev = m_SurfaceInfo->tSubdivisions;
			m_SurfaceInfo->sSubdivisionsPrev = m_SurfaceInfo->sSubdivisions;
		}
		f64 tMin = m_SurfaceInfo->Surface->GetTmin();
		f64 sMin = m_SurfaceInfo->Surface->GetSmin();
		f64 spanLenghtT = m_SurfaceInfo->Surface->GetTmax() - m_SurfaceInfo->Surface->GetTmin();
		f64 spanLenghtS = m_SurfaceInfo->Surface->GetSmax() - m_SurfaceInfo->Surface->GetSmin();
		f64 stepT = spanLenghtT / m_SurfaceInfo->tSubdivisions;
		f64 stepS = spanLenghtS / m_SurfaceInfo->sSubdivisions;
		u64 stepsT = m_SurfaceInfo->tSubdivisions + 1;
		u64 stepsS = m_SurfaceInfo->sSubdivisions + 1;

		{
			HORYZEN_PROFILE_FUNCTION();
			{
				for (size_t i = 0; i < stepsT; ++i) {
					for (size_t j = 0; j < stepsS; ++j) {
						f64 t = tMin + i * stepT;
						f64 s = sMin + j * stepS;
						Vec3D position = m_SurfaceInfo->SurfaceBuilder->CalculatePoint(t, s);
						m_Buffer[i * stepsS + j] = {
							static_cast<f32>(position.x),
							static_cast<f32>(position.y),
							static_cast<f32>(position.z)
						};
					}
				}
			}
		}

	//	m_SurfaceInfo->ShowDerivatives = true;
		if (m_SurfaceInfo->ShowDerivatives) {
			for (size_t i = 0; i < stepsT; ++i) {
				for (size_t j = 0; j < stepsS; ++j) {
					f64 t = tMin + i * stepT;
					f64 s = sMin + j * stepS;
					auto derivative = m_SurfaceInfo->SurfaceBuilder->CalculateFirstDerivative(t, s);

					m_DerivativeBuffer[2 * (i * stepsS + j)] = m_Buffer[i * stepsS + j];

					Vec3D normal = Vec3D::Cross(derivative.first, derivative.second).normalized();

					m_DerivativeBuffer[2 * (i * stepsS + j) + 1] 
						= m_Buffer[i * stepsS + j] + Vec3F(static_cast<f32>(normal.x),
						                                   static_cast<f32>(normal.y),
						                                   static_cast<f32>(normal.z));
				}
			}
		}


		u64 byteSize = (m_SurfaceInfo->tSubdivisions + 1) * (m_SurfaceInfo->sSubdivisions + 1) * 3 * sizeof(f32);
		m_VertexArray->GetVertexBuffers()[0]->UpdateSubData(0, byteSize, m_Buffer.data());

		if (m_SurfaceInfo->ShowDerivatives) {
			u64 derivativeByteSize = (m_SurfaceInfo->tSubdivisions + 1) * (m_SurfaceInfo->sSubdivisions + 1) * 3 * sizeof(f32) * 2;
			m_DerivativeVertexArray->GetVertexBuffers()[0]->UpdateSubData(0, derivativeByteSize, m_DerivativeBuffer.data());
		}
	}


	void GObject::BuildBrokenLineIndexBufferForCurve()
	{
		u64 pointsCount = m_CurveInfo->Curve->GetControlPoints().second.size();
		std::vector<u32> indices;
		/*for (size_t i = 0; i < pointsCount - 1; ++i) {
			u32 index1 = m_GPointCollectionRef->m_IDToStorageIndex[m_GPointIDs[i] - 100];
			u32 index2 = m_GPointCollectionRef->m_IDToStorageIndex[m_GPointIDs[i+1] - 100];
			indices.push_back(index1);
			indices.push_back(index2);
		}*/
		for (size_t i = 0; i < pointsCount; ++i) {
			u32 index = m_GPointCollectionRef->m_IDToStorageIndex[m_GPointIDs[i] - 100];
			indices.push_back(index);
		}
		m_BrokenLineIndexBuffer.reset();
		m_BrokenLineIndexBuffer = IndexBuffer::Create(indices.data(), indices.size());
	}

	void GObject::BuildBrokenLineIndexBufferForSurface()
	{
		u64 pointsT = std::get<2>(m_SurfaceInfo->Surface->GetControlPoints());
		u64 pointsS = std::get<3>(m_SurfaceInfo->Surface->GetControlPoints());

		bool forward = true;
		std::vector<u32> indices;
		for (size_t i = 0; i < pointsT; ++i) {
			if (forward) {
				for (size_t j = 0; j < pointsS; ++j) {
					indices.push_back(m_GPointCollectionRef->m_IDToStorageIndex[m_GPointIDs[i * pointsS + j] - 100]);
				}
			}
			else {
				for (i64 j = pointsS - 1; j >= 0; --j) {
					indices.push_back(m_GPointCollectionRef->m_IDToStorageIndex[m_GPointIDs[i * pointsS + j] - 100]);
				}
			}
			forward = !forward;
		}
		if (forward) {
			forward = false;
			for (size_t j = 0; j < pointsS; ++j) {
				if (forward) {
					for (size_t i = 0; i < pointsT; ++i) {
						indices.push_back(m_GPointCollectionRef->m_IDToStorageIndex[m_GPointIDs[i * pointsS + j] - 100]);
					}
				}
				else {
					for (i64 i = pointsT - 1; i >= 0; --i) {
						indices.push_back(m_GPointCollectionRef->m_IDToStorageIndex[m_GPointIDs[i * pointsS + j] - 100]);
					}
				}
				forward = !forward;
			}

		}
		else {
			for (i64 j = pointsS - 1; j >= 0; --j) {
				if (forward) {
					for (size_t i = 0; i < pointsT; ++i) {
						indices.push_back(m_GPointCollectionRef->m_IDToStorageIndex[m_GPointIDs[i * pointsS + j] - 100]);
					}
				}
				else {
					for (i64 i = pointsT - 1; i >= 0; --i) {
						indices.push_back(m_GPointCollectionRef->m_IDToStorageIndex[m_GPointIDs[i * pointsS + j] - 100]);
					}
				}
				forward = !forward;
			}

		}
		m_BrokenLineIndexBuffer.reset();
		m_BrokenLineIndexBuffer = IndexBuffer::Create(indices.data(), indices.size());
	}

	void GObject::CreateGPoints(std::vector<Vec4D>& p_controlPoints)
	{
		m_GPointIDs.resize(p_controlPoints.size());
		for (size_t i = 0; i < p_controlPoints.size(); ++i) {
			u64 gPointID = m_GPointCollectionRef->AddPoint(&p_controlPoints[i],
				                                  this,
				                                  { 254, 40, 40 });
			m_GPointIDs[i] = gPointID;
		}
	}

}