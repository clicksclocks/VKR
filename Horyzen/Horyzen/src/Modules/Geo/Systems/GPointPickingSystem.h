#pragma once
#include "Horyzen/EntityComponentSystem/System.h"
#include "Horyzen/Math/Vec4.h"
#include "Horyzen/Math/AnalyticalGeometryObjects.h"

namespace Horyzen {
	class FrameBuffer;
	class TransformComponent;
	class CameraComponent;
}


namespace Horyzen::Geo {

	class GPoint;
	class HORYZEN_OBJECT GPointPickingSystem : public ECS::System
	{
	public:

		GPointPickingSystem();

		virtual void UpdateComponents(f32 p_dt, ECS::ComponentBase** p_components) override;

		void SetIDFrameBuffer(std::shared_ptr<FrameBuffer> p_IDFrameBuffer);
		u64* GetIDPointer();
		void SetCameraTransform(TransformComponent* p_cameraTransform);
		void SetCameraComponent(CameraComponent* p_camera);

		void ImGuiPointInfo();

	private:

		void ProceedGPointChange();

		void InitDragPlane();
		void ProcessGPointMovement();
		Line ConstructRayDirectionFromWindowCoordinates();
		void SetUpCurrent();

	private:

		std::shared_ptr<FrameBuffer> m_IDFrameBuffer;
		TransformComponent* m_CameraTransform{ nullptr };
		CameraComponent* m_CameraComponent{ nullptr };
		u64 m_CurrentID{ 0 };
		GPoint* m_CurrentGPoint{ nullptr };

		GPoint* m_LastGPoint{ nullptr };
		u64 m_LastID{ 0 };

		bool m_IsPrevFramePressed{ false };

		Plane m_DragPlane;

		f32 m_Coords[4];
	};

}



