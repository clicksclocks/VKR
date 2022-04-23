#pragma once
#include "Horyzen/EntityComponentSystem/System.h"

namespace Horyzen {
	class CameraComponent;
	class TransformComponent;
	class Shader;
	class FrameBuffer;
}

namespace Horyzen::Geo {

	class HORYZEN_OBJECT GeoRenderSystem : public ECS::System
	{
	public:

		GeoRenderSystem();

		void SetCameraComponent(CameraComponent* p_camera);
		void SetCameraTransform(TransformComponent* p_cameraTransform);
		void SetFrameBuffers(std::shared_ptr<FrameBuffer> p_main,
			                 std::shared_ptr<FrameBuffer> p_ID);

		virtual void UpdateComponents(f32 p_dt, ECS::ComponentBase** p_components) override;

		void ImGuiOptions();

	private:

		std::shared_ptr<Shader> m_PointShader;
		std::shared_ptr<Shader> m_CurveShader;
		std::shared_ptr<Shader> m_SurfaceShader;
		std::shared_ptr<Shader> m_DebugSurfaceShader;

		std::shared_ptr<Shader> m_PointIDShader;

		CameraComponent* m_CameraComponent{ nullptr };
		TransformComponent* m_CameraTransform{ nullptr };

		std::shared_ptr<FrameBuffer> m_MainFrameBuffer;
		std::shared_ptr<FrameBuffer> m_IDFrameBuffer;

		//....................................
		// Options
		bool m_DrawPoints{ true };
		bool m_DrawBrokenLines{ true };

	};

}