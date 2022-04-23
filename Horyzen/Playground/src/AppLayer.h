#pragma once

#include <Horyzen.h>
#include <ImGui/imgui.h>


#include <Horyzen/EntityComponentSystem/Registry.h>

#include "Components/TestComponent.h"

#include <Modules/Geo/Tools/BezierCurveProjector.h>
#include <Modules/Geo/Tools/NURBSCurveProjector.h>


#include <Tools/ComputationalGeometry/CGAlgorithms.h>

class AppLayer : public Horyzen::Layer
{
public:

	virtual void OnUpdate() override
	{
		HORYZEN_PROFILE_FUNCTION();
		using namespace Horyzen;

		f32 PI = 3.1415926535897f;
		m_Rotation = QuatF::fromAxisAndAngle(Vec3F{ 0,0,1 }, 30 * PI / 180);

		m_TRS = Mat4F::ConstructTRSMatrix(
			Vec3F(0.0, 0.0, -2.0),
			m_Rotation,
			Vec3F(1, 1, 1)
		);

		if (InputTracker::IsAltPressed()) {
			m_AttachmentToShow = true;
		} else {
			m_AttachmentToShow = false;
		}

		m_Scene.UpdateSystem(0.01f, m_CameraSystem);
		
		Renderer::BeginScene();
		{
			m_Scene.UpdateSystem(0.01f, m_GeoRenderSystem);
		}
		Renderer::EndScene();

		FrameBuffer::Blit(m_FinalQuadFrameBuffer, 0, m_FrameBuffer, 0);

		RenderCommand::Clear();
		Renderer::BeginScene();
			m_FinalQuadShader->Bind();
			if (m_AttachmentToShow) {
				m_GeoIDsFrameBuffer->BindColorAttachmentToUnit(0, 0);
			} else {
				m_FinalQuadFrameBuffer->BindColorAttachmentToUnit(0, 0);
			}

			Renderer::Submit(m_FinalQuadVertexArray, BasePrimitive::Triangles);
		Renderer::EndScene();

		m_Scene.UpdateSystem(0.01f, m_PickingSystem);

	}

	virtual void OnAttach() override
	{
		HORYZEN_PROFILE_FUNCTION();
		using namespace Horyzen;
		RenderCommand::SetClearColor(0.16, 0.16, 0.16, 1.0);

		f32 vertices[4 * 6 * 6] = {
			// ZF
			 -0.5f, -0.5f, +0.5f, 0.7f, 0.3f, 0.1f,
			 +0.5f, -0.5f, +0.5f, 0.7f, 0.3f, 0.1f,
			 +0.5f, +0.5f, +0.5f, 0.7f, 0.3f, 0.1f,
			 -0.5f, +0.5f, +0.5f, 0.7f, 0.3f, 0.1f,
			 // ZB
			 -0.5f, -0.5f, -0.5f, 0.2f, 0.8f, 0.2f,
			 -0.5f, +0.5f, -0.5f, 0.2f, 0.8f, 0.2f,
			 +0.5f, +0.5f, -0.5f, 0.2f, 0.8f, 0.2f,
			 +0.5f, -0.5f, -0.5f, 0.2f, 0.8f, 0.2f,
			 
			
			 // XB
			 -0.5f, -0.5f, +0.5f, 0.5f, 0.1f, 0.1f,
			 -0.5f, +0.5f, +0.5f, 0.5f, 0.1f, 0.1f,
			 -0.5f, +0.5f, -0.5f, 0.5f, 0.1f, 0.1f,
			 -0.5f, -0.5f, -0.5f, 0.5f, 0.1f, 0.1f,

			 // XF
			 +0.5f, -0.5f, +0.5f, 0.6f, 0.3f, 0.1f,
			 +0.5f, -0.5f, -0.5f, 0.6f, 0.3f, 0.1f,
			 +0.5f, +0.5f, -0.5f, 0.6f, 0.3f, 0.1f,
			 +0.5f, +0.5f, +0.5f, 0.6f, 0.3f, 0.1f,

			 // YB
			 -0.5f, -0.5f, +0.5f, 1.0f, 0.0f, 0.0f,
			 -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			 +0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			 +0.5f, -0.5f, +0.5f, 1.0f, 0.0f, 0.0f,
			 

			 // YF
			 -0.5f, +0.5f, +0.5f, 0.0f, 1.0f, 0.0f,
			 +0.5f, +0.5f, +0.5f, 0.0f, 1.0f, 0.0f,
			 +0.5f, +0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 -0.5f, +0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		};

		m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

		VertexBufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Color" }
		};

		m_VertexBuffer->SetLayout(layout);

		u32 indices[6 * 6] = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};

		m_IndexBuffer = IndexBuffer::Create(indices, 6*6);

		m_VertexArray = std::shared_ptr<VertexArray>(VertexArray::Create());
		m_VertexArray->AddVertexBuffer(std::shared_ptr<VertexBuffer>(m_VertexBuffer));
		m_VertexArray->SetIndexBuffer(std::shared_ptr<IndexBuffer>(m_IndexBuffer));

		std::string shaderVertexSouceCode = R"(
			#version 460 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Color;
			
			layout(location = 0) uniform mat4 u_TRS;
			layout(location = 1) uniform mat4 u_View;
			layout(location = 2) uniform mat4 u_PerspectiveMatrix;

			out vec3 o_color;

			void main()
			{
				o_color = a_Color;
				gl_Position = u_PerspectiveMatrix * u_View * u_TRS * vec4(a_Position, 1.0);
			}
		)";

		std::string shaderFragmentSouceCode = R"(
			#version 460 core

			in vec3 o_color;

			layout(location = 3) uniform vec4 u_Color;

			layout(location = 0) out vec4 color;
			
			void main()
			{
				color = vec4(o_color, 1.0);
			}
		)";

		m_Shader = Shader::Create(shaderVertexSouceCode, shaderFragmentSouceCode);


		auto camera = m_Scene.AddEntity();
		m_CameraEntityID = camera.GetEntityID();
		TransformComponent& camTr = camera.AddComponent<TransformComponent>();
		camTr.Position.x = 5.6;
		camTr.Position.y = 6;
		camTr.Position.z = 25;
		CameraComponent& camCam = camera.AddComponent<CameraComponent>();
		camCam.SetProjectionMatrix(Mat4F::ConstructPerspectiveMatrix(0.785398f, 16.f / 9.f, 0.1f, 100.f));

		FrameBufferSpecification frameBufferSpec;
		frameBufferSpec.Samples = 8;
		frameBufferSpec.Width = Horyzen::Application::Get().GetWindow().GetWidth();
		frameBufferSpec.Height = Horyzen::Application::Get().GetWindow().GetHeight();
		frameBufferSpec.AttachmentSpecs = {FrameBufferAttachmentFormat::RGBA8,
		                                   FrameBufferAttachmentFormat::Depth24Stencil8};

		m_FrameBuffer = FrameBuffer::Create(frameBufferSpec);

		

		// Final Quad concept
		FrameBufferSpecification finalQuadFrameBufferSpec;
		finalQuadFrameBufferSpec.Samples = 1;
		finalQuadFrameBufferSpec.Width = Horyzen::Application::Get().GetWindow().GetWidth();
		finalQuadFrameBufferSpec.Height = Horyzen::Application::Get().GetWindow().GetHeight();
		finalQuadFrameBufferSpec.AttachmentSpecs = { FrameBufferAttachmentFormat::RGBA8 };
		m_FinalQuadFrameBuffer = FrameBuffer::Create(finalQuadFrameBufferSpec);

		FrameBufferSpecification IDFrameBufferSpec;
		IDFrameBufferSpec.Samples = 1;
		IDFrameBufferSpec.Width = Horyzen::Application::Get().GetWindow().GetWidth();
		IDFrameBufferSpec.Height = Horyzen::Application::Get().GetWindow().GetHeight();
		IDFrameBufferSpec.AttachmentSpecs = { FrameBufferAttachmentFormat::RGBA8,
		                                      FrameBufferAttachmentFormat::Depth24Stencil8 };

		m_GeoIDsFrameBuffer = FrameBuffer::Create(IDFrameBufferSpec);

		
		std::string finalQuadShaderVertexSouceCode = R"(
			#version 460 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_UV;
			
			out vec2 o_UV;

			void main()
			{
				o_UV = a_UV;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string finalQuadSFragmentSouceCode = R"(
			#version 460 core

			in vec2 o_UV;

			layout(binding = 0) uniform sampler2D sampler;

			layout(location = 0) out vec4 color;
			
			void main()
			{
				color = texture(sampler, o_UV); 
				//color = vec4(1.0, 0.0, 0.0, 1.0); 
			}
		)";

		m_FinalQuadShader = Shader::Create(finalQuadShaderVertexSouceCode, finalQuadSFragmentSouceCode);


		f32 finalQuadVertices[4 * 5] = {
			 -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 +1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			 +1.0f, +1.0f, 0.0f, 1.0f, 1.0f,
			 -1.0f, +1.0f, 0.0f, 0.0f, 1.0f
		};

		u32 finalQuadIndices[6] = {
			0, 1, 2, 0, 2, 3
		};

		m_FinalQuadVertexBuffer = VertexBuffer::Create(finalQuadVertices, sizeof(finalQuadVertices));

		VertexBufferLayout finalQuadLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_UV" }
		};

		m_FinalQuadVertexBuffer->SetLayout(finalQuadLayout);

		m_FinalQuadIndexBuffer = IndexBuffer::Create(indices, 6);

		m_FinalQuadVertexArray = VertexArray::Create();;
		m_FinalQuadVertexArray->AddVertexBuffer(m_FinalQuadVertexBuffer);
		m_FinalQuadVertexArray->SetIndexBuffer(m_FinalQuadIndexBuffer);

		// Geo stuff
		auto geoEntity = m_Scene.AddEntity();
		m_GeoEntityID = geoEntity.GetEntityID();
		Geo::Geo& geo = geoEntity.AddComponent<Geo::Geo>();
		m_Geo = &geo;
		m_GeoRenderSystem.SetCameraComponent(&camCam);
		m_GeoRenderSystem.SetCameraTransform(&camTr);
		m_GeoRenderSystem.SetFrameBuffers(m_FrameBuffer, m_GeoIDsFrameBuffer);
		m_PickingSystem.SetIDFrameBuffer(m_GeoIDsFrameBuffer);
		m_PickingSystem.SetCameraTransform(&camTr);
		m_PickingSystem.SetCameraComponent(&camCam);

		// Geo curves and surfaces
		using namespace Horyzen::Geo;
		auto curve1 = std::make_shared<NURBSCurve>();
		//auto curve2 = std::make_shared<NURBSCurve>();
		//curve2->DecomposeIntoBezier();
		m_Geo->AddCurve(curve1);
		//m_Geo->AddCurve(curve2);

	    auto surface1 = std::make_shared<NURBSSurface>();
		//auto surface2 = std::make_shared<NURBSSurface>();
		//surface2->DecomposeIntoBezier();
		m_Geo->AddSurface(surface1);
		//m_Geo->AddSurface(surface2);





		//......................................................................
		//......................................................................


	//	auto curve = std::make_shared<BezierCurve>();
	//	m_Geo->AddCurve(curve);
	//	BezierCurveProjector projector(curve);

		//m_NURBSCurve = std::make_shared<NURBSCurve>();
		//NURBSCurveProjector nurbsProjector(m_NURBSCurve);
		//m_Geo->AddCurve(m_NURBSCurve);

		//std::vector<Vec4D> testPoints;

		//u64 tSubdivs = m_tPoints - 1;
		//u64 sSubdivs = m_sPoints - 1;

		//f64 tStep = m_xSpan / tSubdivs;
		//f64 sStep = m_ySpan / sSubdivs;

		//for (size_t i = 0; i < m_tPoints; ++i) {
		//	for (size_t j = 0; j < m_sPoints; ++j) {
		//		const Vec2D point(m_xMin + i * tStep,
  //                                m_yMin + j * sStep);
		//		//f64 value = projector.GetDistance(x, y);
		//		f64 value = nurbsProjector.GetDistance1stOrderApproximation(point);
		//		testPoints.push_back({ point.x, point.y, -1, value });
		//	}
		//}
		//m_Geo->AddDebugSurface(testPoints, m_tPoints, m_sPoints, m_MaxLevels, m_Multiplier);


		//size_t N = 30;
		//f64 step = 16.0 / (N - 1);


		//for (size_t i = 0; i < N; ++i) {
		//	const Vec2D p0(-8.0 + i* step, 6.0);
		//	auto p1 = nurbsProjector.Project1stOrderApproximation(p0);
		//	std::vector<Vec4D> pnts(2);
		//	pnts[0] = Vec4D(p0.x, p0.y, -1, 1);
		//	pnts[1] = Vec4D(p1.x, p1.y, -1, 1);

		//	auto bzcurve = std::make_shared<BezierCurve>(pnts, 0, 1);
		//	m_Geo->AddCurve(bzcurve);
		//}

	}

	virtual void OnImGuiRender() override
	{
		using namespace Horyzen;
		HORYZEN_PROFILE_FUNCTION();

		//ImGui::Begin("Welcome");
		//	ImGui::Text("Hello!");
		//ImGui::End();

		ImGui::Begin("Camera");
		{
	//		CameraComponent& camCam = m_Scene.GetComponent<CameraComponent>(m_CameraEntityID);
			TransformComponent& camTr = m_Scene.GetComponent<TransformComponent>(m_CameraEntityID);

			ImGui::DragFloat3("Camera position", &camTr.Position.x, -5.f, 5.f);
		}
		ImGui::End();


		//ImGui::Begin("Picture Color");
		//	u32 texID = m_FinalQuadFrameBuffer->GetColorAttachmentID(0);
		//	ImGui::Image((void*)texID, ImVec2{ 1600, 900 }, ImVec2(0, 1), ImVec2(1, 0));
		//ImGui::End();
		//ImGui::Begin("Picture ID");
		//	texID = m_GeoIDsFrameBuffer->GetColorAttachmentID(0);
		//	ImGui::Image((void*)texID, ImVec2{ 1600, 900 }, ImVec2(0, 1), ImVec2(1, 0));
		//ImGui::End();
		
		ImGui::Begin("Color Picker");
			u8 color[3] = {0, 0, 0};
			Horyzen::RenderCommand::ReadPixelFromScreenBuffer(
				Horyzen::InputTracker::GetMousePosX(),
				Horyzen::Application::Get().GetWindow().GetHeight() - Horyzen::InputTracker::GetMousePosY(),
				m_ScreenColor);
			ImGui::ColorEdit3("color", m_ScreenColor);
			ImGui::Checkbox("Show ID Attachment", &m_AttachmentToShow);

			
		ImGui::End();

		ImGui::Begin("GPoint info");
		{
			i32 ID = static_cast<i32>(*m_PickingSystem.GetIDPointer());
			ImGui::InputInt("Current GPoint selected", &ID);
			m_PickingSystem.ImGuiPointInfo();
		}
		ImGui::End();

		ImGui::Begin("Geo renderer options");
		{
			m_GeoRenderSystem.ImGuiOptions();
		}
		ImGui::End();

		ImGui::Begin("Rebuilder");
			if (ImGui::Button("Rebuild level sets", {150, 40})) {
				using namespace Horyzen::Geo;
				m_Geo->ClearDebugObjectStorage();
				NURBSCurveProjector nurbsProjector(m_NURBSCurve);

				std::vector<Vec4D> testPoints;

				u64 tSubdivs = m_tPoints - 1;
				u64 sSubdivs = m_sPoints - 1;

				f64 tStep = m_xSpan / tSubdivs;
				f64 sStep = m_ySpan / sSubdivs;

				for (i32 i = 0; i < m_tPoints; ++i) {
					for (i32 j = 0; j < m_sPoints; ++j) {
						const Vec2D point(m_xMin + i * tStep,
						                  m_yMin + j * sStep);

						f64 value;
						if (m_Switch)
							value = nurbsProjector.GetDistance2ndOrderApproximation(point);
						else
							value = nurbsProjector.GetDistance1stOrderApproximation(point);
						testPoints.push_back({ point.x, point.y, -1, value });
					}
				}
				m_Geo->AddDebugSurface(testPoints, m_tPoints, m_sPoints, m_MaxLevels, m_Multiplier);
			}
			ImGui::Checkbox("Do second order?", &m_Switch);
			
			ImGui::InputInt("tPoints", &m_tPoints);
			ImGui::InputInt("sPoints", &m_sPoints);
			ImGui::InputDouble("xMin", &m_xMin);
			ImGui::InputDouble("xSpan", &m_xSpan);
			ImGui::InputDouble("yMin", &m_yMin);
			ImGui::InputDouble("ySpan", &m_ySpan);
			ImGui::InputInt("Max levels", &m_MaxLevels);
			ImGui::InputInt("Multiplier", &m_Multiplier);

		ImGui::End();
	}

	virtual void OnWindowResizedEvent(Horyzen::WindowResizedEvent& p_event) override
	{
		if (p_event.GetWidth() == 0 || p_event.GetHeight() == 0) {
			return;
		}
		m_FrameBuffer->Resize(p_event.GetWidth(), p_event.GetHeight());
		m_FinalQuadFrameBuffer->Resize(p_event.GetWidth(), p_event.GetHeight());
		m_GeoIDsFrameBuffer->Resize(p_event.GetWidth(), p_event.GetHeight());

		Horyzen::CameraComponent& camCam = m_Scene.GetComponent<Horyzen::CameraComponent>(m_CameraEntityID);
		camCam.ProjectionMatrix = Mat4F::ConstructPerspectiveMatrix(0.785398f, f32(p_event.GetWidth()) / p_event.GetHeight(), 0.1f, 100.f);
		Horyzen::Application::Get().GetWindow().SetViewPortForRenderingContext(0, 0, p_event.GetWidth(), p_event.GetHeight());
	}

private:

	// Debug surface data
	i32 m_tPoints = 200;
	i32 m_sPoints = 200;

	f64 m_xMin = -8.0;
	f64 m_yMin = -3.0;

	f64 m_xSpan = 16.0;
	f64 m_ySpan = 16.0;

	i32 m_MaxLevels = 7;
	i32 m_Multiplier = 1;

	//........................

	bool m_Switch = false;

	std::shared_ptr<Horyzen::Geo::NURBSCurve> m_NURBSCurve;

	std::shared_ptr<Horyzen::IndexBuffer> m_IndexBuffer;
	std::shared_ptr<Horyzen::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Horyzen::VertexArray> m_VertexArray;
	std::shared_ptr<Horyzen::Shader> m_Shader;

	std::shared_ptr<Horyzen::FrameBuffer> m_FrameBuffer;
	std::shared_ptr<Horyzen::FrameBuffer> m_FinalQuadFrameBuffer;
	std::shared_ptr<Horyzen::FrameBuffer> m_GeoIDsFrameBuffer;

	std::shared_ptr<Horyzen::IndexBuffer>  m_FinalQuadIndexBuffer;
	std::shared_ptr<Horyzen::VertexBuffer> m_FinalQuadVertexBuffer;
	std::shared_ptr<Horyzen::VertexArray>  m_FinalQuadVertexArray;
	std::shared_ptr<Horyzen::Shader>       m_FinalQuadShader;


	f32 m_ScreenColor[3];

	Mat4F m_TRS;

	QuatF m_Rotation;

	Horyzen::CameraSystem m_CameraSystem;

	Horyzen::ECS::EntityIDType m_CameraEntityID;

	Horyzen::Scene m_Scene;

	// Geo stuff
	Horyzen::ECS::EntityIDType m_GeoEntityID;
	Horyzen::Geo::Geo* m_Geo;
	Horyzen::Geo::GeoRenderSystem m_GeoRenderSystem;
	Horyzen::Geo::GPointPickingSystem m_PickingSystem;

	bool m_AttachmentToShow = false;

};