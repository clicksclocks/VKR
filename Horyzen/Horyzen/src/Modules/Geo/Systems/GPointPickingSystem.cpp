#include "pchheader.h"
#include "Horyzen/Input/InputTracker.h"
#include "GPointPickingSystem.h"
#include "../Geo.h"
#include "Horyzen/Renderer/FrameBuffer.h"
#include "Horyzen/Application.h"
#include "Horyzen/Logger.h"
#include "Horyzen/Components/TransformComponent.h"
#include "Horyzen/Components/CameraComponent.h"
#include "Horyzen/Math/Mat4.h"

#include <imgui.h>


namespace Horyzen::Geo {

	GPointPickingSystem::GPointPickingSystem()
	{
		AddComponentType(Geo::GetComponentID());
	}

	void GPointPickingSystem::UpdateComponents(f32 p_dt, ECS::ComponentBase** p_components)
	{
		Geo* geo = static_cast<Geo*>(p_components[0]);
		
		if (InputTracker::IsLeftMouseButtonPressed()) {
			if (!m_IsPrevFramePressed) {
				auto color = m_IDFrameBuffer->ReadPixelRGBu(
					0,
					InputTracker::GetMousePosX(),
					Horyzen::Application::Get().GetWindow().GetHeight() - Horyzen::InputTracker::GetMousePosY());
				m_CurrentID = GPoint::ColorIDToID(color);

			//	HORYZEN_LOG_WARN("R: {0},  G: {1}, B: {2},  ID: {3}", color.r, color.g, color.b, m_CurrentID);
				if (m_CurrentID < 100) {
					m_CurrentGPoint = nullptr;
				} else {
					u64 storageIndex = geo->m_GPointCollection.m_IDToStorageIndex[m_CurrentID-100];
					m_CurrentGPoint = &geo->m_GPointCollection.m_GPointStorage[storageIndex];
					m_LastGPoint = &geo->m_GPointCollection.m_GPointStorage[storageIndex];
					m_LastID = m_LastGPoint->m_ID;
					InitDragPlane();
					SetUpCurrent();
				}
			} else {
				if (m_CurrentGPoint) {
					ProcessGPointMovement();
				}
			}
			m_IsPrevFramePressed = true;
		} else {
			m_IsPrevFramePressed = false;
			ProceedGPointChange();
		}
	}

	void GPointPickingSystem::SetIDFrameBuffer(std::shared_ptr<FrameBuffer> p_IDFrameBuffer)
	{
		m_IDFrameBuffer = p_IDFrameBuffer;
	}

	u64* GPointPickingSystem::GetIDPointer()
	{
		return &m_CurrentID;
	}

	void GPointPickingSystem::SetCameraTransform(TransformComponent* p_cameraTransform)
	{
		m_CameraTransform = p_cameraTransform;
	}

	void GPointPickingSystem::SetCameraComponent(CameraComponent* p_camera)
	{
		m_CameraComponent = p_camera;
	}

	void GPointPickingSystem::ImGuiPointInfo()
	{
		if (m_LastGPoint != nullptr) {
			ImGui::DragFloat4("GPoint info", &m_Coords[0], -1.0f, 1.0f);
		}
	}


	void GPointPickingSystem::ProceedGPointChange()
	{
		
		if (m_LastGPoint) {
			Vec4D currentCoords = *m_LastGPoint->m_References[0].second;
			if (currentCoords.x != m_Coords[0] || currentCoords.y != m_Coords[1] ||
				currentCoords.z != m_Coords[2] || currentCoords.w != m_Coords[3]) {
				for (auto& ref : m_LastGPoint->m_References) {
					Vec4D* vecView = ref.second;
					vecView->x = m_Coords[0];
					vecView->y = m_Coords[1];
					vecView->z = m_Coords[2];
					vecView->w = m_Coords[3];
					ref.first->Update();
				}
			}
			m_LastGPoint->m_CollectionReference->UpdatePointInBuffer(m_LastID);
		}	
	}

	void GPointPickingSystem::InitDragPlane()
	{
		Vec4D vec4 = *m_CurrentGPoint->m_References[0].second;
		Vec3D position = { vec4.x, vec4.y, vec4.z };
		Vec3D vecForward = {
			m_CameraTransform->GetForwardVector().x,
		    m_CameraTransform->GetForwardVector().y,
		    m_CameraTransform->GetForwardVector().z
		};
		m_DragPlane = Plane(position, vecForward.normalized());
	}

	void GPointPickingSystem::ProcessGPointMovement()
	{
		Line ray = ConstructRayDirectionFromWindowCoordinates();
		f64 tParam = m_DragPlane.FindParameterOfIntersection(ray);
		Vec3D newPosition = ray(tParam);
		for (auto ref : m_CurrentGPoint->m_References) {
			Vec4D* vecView = ref.second;
			vecView->x = newPosition.x;
			vecView->y = newPosition.y;
			vecView->z = newPosition.z;
			ref.first->Update();
		}

		m_CurrentGPoint->m_CollectionReference->UpdatePointInBuffer(m_CurrentID);
		SetUpCurrent();
	}

	Line GPointPickingSystem::ConstructRayDirectionFromWindowCoordinates()
	{
		u32 windowX = InputTracker::GetMousePosX();
		u32 windowY = Horyzen::Application::Get().GetWindow().GetHeight() - Horyzen::InputTracker::GetMousePosY();
		u32 windowWidth  = Horyzen::Application::Get().GetWindow().GetWidth();
		u32 windowHeight = Horyzen::Application::Get().GetWindow().GetHeight();
		
		// ......................................................
		// Obtaining normallized coordinates
		f32 normalizedX = (windowX * 1.0f / windowWidth) * 2 - 1;
		f32 normalizedY = (windowY * 1.0f / windowHeight) * 2 - 1;

		// ......................................................
		// 4D Homogeneous clip coordinates
		Vec4F homogeneousClipCoords(normalizedX, normalizedY, -1.0, 1.0);

		// ......................................................
		// 4D Eye (Camera) coordinates
		Mat4F invertedProjectionMatrix = m_CameraComponent->ProjectionMatrix.inversed();
		Vec4F eyeCoords = invertedProjectionMatrix * homogeneousClipCoords;
		eyeCoords.z = -1.0f;
		eyeCoords.w = 0.0f;

		// ......................................................
		// 4D World coordinates
		Vec4F worldCoords = m_CameraTransform->Transform * eyeCoords;

		Vec3D ray = { worldCoords.x, worldCoords.y, worldCoords.z };
		ray.normalize();

		Vec3D eye(
			static_cast<f64>(m_CameraTransform->Position.x),
			static_cast<f64>(m_CameraTransform->Position.y),
			static_cast<f64>(m_CameraTransform->Position.z)
		);

		Line o_line(eye, ray);
		return o_line;
	}

	void GPointPickingSystem::SetUpCurrent()
	{
		m_Coords[0] = m_LastGPoint->m_References[0].second->x;
		m_Coords[1] = m_LastGPoint->m_References[0].second->y;
		m_Coords[2] = m_LastGPoint->m_References[0].second->z;
		m_Coords[3] = m_LastGPoint->m_References[0].second->w;
	}

}
