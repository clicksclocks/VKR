#include "pchheader.h"
#include "CameraComponent.h"

namespace Horyzen {

	void CameraComponent::SetProjectionMatrix(const Mat4F& p_projectionMatrix)
	{
		ProjectionMatrix = p_projectionMatrix;
	}

}