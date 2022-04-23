#pragma once

#include "Horyzen/Core.h"
#include "Horyzen/Math/BasicMath.h"

namespace Horyzen {

	class Camera
	{
	public:
		
		Camera() = default;
		Camera(const Mat4F& p_projectionMatrix)
			: m_ProjectionMatrix(p_projectionMatrix)
		{}

		void SetPerspective(f32 p_fov, f32 p_aspectRatio, f32 p_nearZ, f32 p_farZ)
		{
			m_ProjectionMatrix = Mat4F::ConstructPerspectiveMatrix(p_fov, p_aspectRatio, p_nearZ, p_farZ);
		}

		const Mat4F& GetProjectionMatrix() { return m_ProjectionMatrix; }

	private:

		Mat4F m_ProjectionMatrix{ 1.0f };

	};

}