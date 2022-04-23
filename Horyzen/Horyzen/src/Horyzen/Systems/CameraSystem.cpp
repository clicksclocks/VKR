#include "pchheader.h"
#include "CameraSystem.h"

#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"

#include "../Logger.h"
#include "../Input/InputTracker.h"

#include "../EntityComponentSystem/Component.h"

namespace Horyzen {

	CameraSystem::CameraSystem()
	{
		AddComponentType(CameraComponent::GetComponentID());
		AddComponentType(TransformComponent::GetComponentID());
	}

	void CameraSystem::UpdateComponents(f32 p_dt, ECS::ComponentBase** p_components)
	{
		CameraComponent* cameraComp = static_cast<CameraComponent*>(p_components[0]);
		TransformComponent* transformComp = static_cast<TransformComponent*>(p_components[1]);

		//HORYZEN_LOG_WARN("x: {0}, y: {1}, z: {2}", transformComp->Position.x,
		//	                                       transformComp->Position.y,
		//	                                       transformComp->Position.z);

		{

			f32 speed = 0.03f;
			if (InputTracker::IsKeyPressed(Key::C)) speed *= 4;

			if (Horyzen::InputTracker::IsKeyPressed(Key::W)) {
				Vec3F Forward = transformComp->GetForwardVector().normalized();
				transformComp->Translate(Forward * -speed);
			}
			if (Horyzen::InputTracker::IsKeyPressed(Key::A)) {
				Vec3F Left = transformComp->GetLeftVector().normalized();
				transformComp->Translate(Left * speed);
			}
			if (Horyzen::InputTracker::IsKeyPressed(Key::S)) {
				Vec3F Backward = transformComp->GetBackwardVector().normalized();
				transformComp->Translate(Backward * -speed);
			}
			if (Horyzen::InputTracker::IsKeyPressed(Key::D)) {
				Vec3F Right = transformComp->GetRightVector().normalized();
				transformComp->Translate(Right * speed);
			}
			if (Horyzen::InputTracker::IsRightMouseButtonPressed()) {

				auto dy = Horyzen::InputTracker::GetMouseDY();
				auto dx = Horyzen::InputTracker::GetMouseDX();

				if (Horyzen::InputTracker::IsLeftShiftPressed()) {
					Vec3F Up = transformComp->GetUpVector().normalized();
					transformComp->Translate(Up * 0.003f * dy);
				}
				else {
					m_CameraAngleX -= 0.04f * dy;
					m_CameraAngleY -= 0.04f * dx;

					f32 PI = 3.1415926535897f;

					QuatF yQuatRot = QuatF::fromAxisAndAngle(Vec3F{ 0.0f, 1.0f, 0.0f }, m_CameraAngleY * PI / 180);
					Vec3F forwardVecDirection = Vec3F{ 0.0f, 0.0f, -1.0 }.rotated(yQuatRot);

					Vec3F Right = Vec3F::Cross(forwardVecDirection, Vec3F{ 0.0f, 1.0f, 0.0f });

					QuatF xQuatRot = QuatF::fromAxisAndAngle(Right, m_CameraAngleX * PI / 180);

					transformComp->Rotation = xQuatRot * yQuatRot;
				}
			}
		}

		transformComp->UpdateTransform();
		cameraComp->View = transformComp->Transform.inversed();

	}

	

}
