#pragma once

#include "../EntityComponentSystem/System.h"


namespace Horyzen {

	class HORYZEN_OBJECT CameraSystem : public ECS::System
	{
	public:

		CameraSystem();

		virtual void UpdateComponents(f32 p_dt, ECS::ComponentBase** p_components) override;

	private:

		f32 m_CameraAngleX = 0;
		f32 m_CameraAngleY = 0;

	};

}

