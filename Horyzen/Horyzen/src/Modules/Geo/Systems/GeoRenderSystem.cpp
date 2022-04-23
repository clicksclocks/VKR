#include "pchheader.h"
#include "GeoRenderSystem.h"
#include "../Geo.h"

#include "Horyzen/Renderer/Shader.h"
#include "Horyzen/Renderer/VertexArray.h"
#include "Horyzen/Renderer/Renderer.h"
#include "Horyzen/Renderer/FrameBuffer.h"
#include "Horyzen/Components/CameraComponent.h"
#include "Horyzen/Components/TransformComponent.h"
#include "Horyzen/Input/InputTracker.h"

#include <imgui.h>

namespace Horyzen::Geo {

	GeoRenderSystem::GeoRenderSystem()
	{
		AddComponentType(Geo::GetComponentID());

		std::string pointShaderVertexSouceCode = R"(
			#version 460 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Color;
			layout(location = 2) in vec3 a_ColorID;
			
			layout(location = 0) uniform mat4 u_View;
			layout(location = 1) uniform mat4 u_PerspectiveMatrix;
			layout(location = 2) uniform vec3 u_Eye;

			out vec3 o_color;
            out vec3 o_colorID;

			void main()
			{
				float d = distance(u_Eye.xyz,a_Position.xyz);
				if(d > 0.0f) gl_PointSize = max(8.0f,(50.0f/d));
				else gl_PointSize = 8.0f;

				o_color = a_Color;
				o_colorID = a_ColorID;
                //gl_PointSize = 30;
				gl_Position = u_PerspectiveMatrix * u_View * vec4(a_Position, 1.0);
			}
		)";

		std::string pointShaderFragmentSouceCode = R"(
			#version 460 core

			in vec3 o_color;
			in vec3 o_colorID;

			layout(location = 0) out vec4 color;
			layout(location = 1) out vec4 colorID;
		
			void main()
			{
				color = vec4(o_color, 1.0);
                colorID = vec4(o_colorID, 1.0);
			}
		)";

		m_PointShader = Shader::Create(pointShaderVertexSouceCode, 
			                           pointShaderFragmentSouceCode);

		std::string pointIDShaderVertexSouceCode = R"(
			#version 460 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Color;
			layout(location = 2) in vec3 a_ColorID;
			
			layout(location = 0) uniform mat4 u_View;
			layout(location = 1) uniform mat4 u_PerspectiveMatrix;
			layout(location = 2) uniform vec3 u_Eye;

			out vec3 o_color;
            out vec3 o_colorID;

			void main()
			{
				float d = distance(u_Eye.xyz,a_Position.xyz);
				if(d > 0.0f) gl_PointSize = max(8.0f,(50.0f/d));
				else gl_PointSize = 8.0f;

				o_colorID = a_ColorID;
				gl_Position = u_PerspectiveMatrix * u_View * vec4(a_Position, 1.0);
			}
		)";

		std::string pointIDShaderFragmentSouceCode = R"(
			#version 460 core

			in vec3 o_colorID;

			layout(location = 0) out vec4 colorID;
		
			void main()
			{
                colorID = vec4(o_colorID, 1.0);
			}
		)";

		m_PointIDShader = Shader::Create(pointIDShaderVertexSouceCode,
			                           pointIDShaderFragmentSouceCode);

		std::string curveShaderVertexSouceCode = R"(
			#version 460 core

			layout(location = 0) in vec3 a_Position;
						
			layout(location = 0) uniform mat4 u_View;
			layout(location = 1) uniform mat4 u_PerspectiveMatrix;


			void main()
			{
				gl_PointSize = 30;
				gl_Position = u_PerspectiveMatrix * u_View * vec4(a_Position, 1.0);
			}
		)";

		std::string curveShaderFragmentSouceCode = R"(
			#version 460 core

			layout(location = 2) uniform vec3 u_Color;

			layout(location = 0) out vec4 color;
			layout(location = 1) out vec4 colorS;
		
			void main()
			{
				color = vec4(u_Color, 1.0);
				colorS = vec4(0.0, 0.0, 0.0, 1.0);
			}
		)";

		m_CurveShader = Shader::Create(curveShaderVertexSouceCode,
			                           curveShaderFragmentSouceCode);


		std::string debugSurfaceShaderVertexSouceCode = R"(
			#version 460 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Color;
			
			layout(location = 0) uniform mat4 u_View;
			layout(location = 1) uniform mat4 u_PerspectiveMatrix;

			out vec3 o_color;

			void main()
			{
				o_color = a_Color;
				gl_PointSize = 30;
				gl_Position = u_PerspectiveMatrix * u_View * vec4(a_Position, 1.0);
			}
		)";

		std::string debugSurfaceShaderFragmentSouceCode = R"(
			#version 460 core

			in vec3 o_color;

			layout(location = 0) out vec4 color;
		
			void main()
			{
				color = vec4(o_color, 1.0);
			}
		)";

		m_DebugSurfaceShader = Shader::Create(debugSurfaceShaderVertexSouceCode,
			                                  debugSurfaceShaderFragmentSouceCode);


	}

	void GeoRenderSystem::SetCameraComponent(CameraComponent* p_camera)
	{
		m_CameraComponent = p_camera;
	}

	void GeoRenderSystem::SetCameraTransform(TransformComponent* p_cameraTransform)
	{
		m_CameraTransform = p_cameraTransform;
	}

	void GeoRenderSystem::SetFrameBuffers(std::shared_ptr<FrameBuffer> p_main,
		                                  std::shared_ptr<FrameBuffer> p_ID)
	{
		m_MainFrameBuffer = p_main;
		m_IDFrameBuffer = p_ID;
	}

	void GeoRenderSystem::UpdateComponents(f32 p_dt, ECS::ComponentBase** p_components)
	{
		Geo* geo = static_cast<Geo*>(p_components[0]);

		//..........................................
		// Main buffer draw
		m_MainFrameBuffer->Bind();

		RenderCommand::Clear();
		m_MainFrameBuffer->ClearColorAttachment(0, 0.2, 0.2, 0.2, 1.0);

		m_PointShader->Bind();
		m_PointShader->SetUniformMatrix4f(0, &m_CameraComponent->View.m0);
		m_PointShader->SetUniformMatrix4f(1, &m_CameraComponent->ProjectionMatrix.m0);
		Vec3F eye(
			m_CameraTransform->Position.x,
			m_CameraTransform->Position.y,
			m_CameraTransform->Position.z
		);
		m_PointShader->SetUniform3f(2, eye.rawData);
		if (m_DrawPoints) {
			Renderer::Submit(geo->m_GPointCollection.m_VertexArray, BasePrimitive::Points);
		}

		m_CurveShader->Bind();
		m_CurveShader->SetUniformMatrix4f(0, &m_CameraComponent->View.m0);
		m_CurveShader->SetUniformMatrix4f(1, &m_CameraComponent->ProjectionMatrix.m0);

		for (auto gObject : geo->m_GeometryObjects) {
			switch (gObject->m_Mode) {
				case GObject::Mode::Curve: {
					m_CurveShader->Bind();

					f32 color[3] = { gObject->m_Color.r / 255.0f,
									 gObject->m_Color.g / 255.0f,
									 gObject->m_Color.b / 255.0f };

					m_CurveShader->SetUniform3f(2, color);
				} break;

				case GObject::Mode::Surface: {
					m_CurveShader->Bind();

					f32 color[3] = { gObject->m_Color.r / 255.0f,
									 gObject->m_Color.g / 255.0f,
									 gObject->m_Color.b / 255.0f };

					m_CurveShader->SetUniform3f(2, color);
				} break;
			}
			if (!InputTracker::IsKeyPressed(Key::H)) {
				gObject->Draw();
				if (gObject->m_Mode == GObject::Mode::Curve || gObject->m_Mode == GObject::Mode::Surface) {
					f32 color[3] = { gObject->m_DerivativeColor.r / 255.0f,
									 gObject->m_DerivativeColor.g / 255.0f,
									 gObject->m_DerivativeColor.b / 255.0f };

					m_CurveShader->SetUniform3f(2, color);
					gObject->DrawDerivative();
				}
			}
			
			switch (gObject->m_Mode) {
				case GObject::Mode::Curve: {
					m_CurveShader->Bind();

					f32 color[3] = { gObject->m_BrokenLineColor.r / 255.0f,
									 gObject->m_BrokenLineColor.g / 255.0f,
									 gObject->m_BrokenLineColor.b / 255.0f };

					m_CurveShader->SetUniform3f(2, color);
				} break;

				case GObject::Mode::Surface: {
					m_CurveShader->Bind();

					f32 color[3] = { gObject->m_BrokenLineColor.r / 255.0f,
									 gObject->m_BrokenLineColor.g / 255.0f,
									 gObject->m_BrokenLineColor.b / 255.0f };

					m_CurveShader->SetUniform3f(2, color);
				} break;
			}
			if (m_DrawBrokenLines) {
				if (!InputTracker::IsKeyPressed(Key::H)) gObject->DrawBrokenLine();
			}
		}

		m_DebugSurfaceShader->Bind();
		m_DebugSurfaceShader->SetUniformMatrix4f(0, &m_CameraComponent->View.m0);
		m_DebugSurfaceShader->SetUniformMatrix4f(1, &m_CameraComponent->ProjectionMatrix.m0);

		for (auto debugSurface : geo->m_DebugObjects) {
			debugSurface->Draw();
		}


		//..........................................
		// ID buffer draw
		m_IDFrameBuffer->Bind();

		RenderCommand::Clear();
		m_IDFrameBuffer->ClearColorAttachment(0, 0.0, 0.0, 0.0, 1.0);

		m_PointIDShader->Bind();
		m_PointIDShader->SetUniformMatrix4f(0, &m_CameraComponent->View.m0);
		m_PointIDShader->SetUniformMatrix4f(1, &m_CameraComponent->ProjectionMatrix.m0);
		m_PointIDShader->SetUniform3f(2, eye.rawData);
		if (m_DrawPoints) {
			Renderer::Submit(geo->m_GPointCollection.m_VertexArray, BasePrimitive::Points);
		}

		m_IDFrameBuffer->Unbind();
	}

	void GeoRenderSystem::ImGuiOptions()
	{
		ImGui::Checkbox("Show GPoints", &m_DrawPoints);
		ImGui::Checkbox("Show broken lines", &m_DrawBrokenLines);
	}

}
