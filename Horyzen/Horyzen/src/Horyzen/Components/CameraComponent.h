#pragma once
#include "../EntityComponentSystem/Component.h"
#include "Horyzen/Math/BasicMath.h"

namespace Horyzen {

	struct HORYZEN_OBJECT CameraComponent : public ECS::Component<CameraComponent>
	{

		void SetProjectionMatrix(const Mat4F& p_projectionMatrix);

		Mat4F ProjectionMatrix;
		Mat4F View;

	};

}

