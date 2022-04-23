#pragma once

#include "Horyzen/Math/BasicMath.h"
#include "Horyzen/Core.h"
#include "../EntityComponentSystem/Component.h"

namespace Horyzen {

	struct HORYZEN_OBJECT TransformComponent : public Horyzen::ECS::Component<TransformComponent>
	{
		Vec3F Position;
		QuatF Rotation;
		Vec3F Scale{ 1.0f, 1.0f, 1.0f };

		Mat4F Transform;

		Vec3F GetRightVector() const
		{
			return Vec3F{
				Transform.m0,
				Transform.m1,
				Transform.m2
			};
		}

		Vec3F GetUpVector() const
		{
			return Vec3F{
				Transform.m4,
				Transform.m5,
				Transform.m6
			};
		}

		Vec3F GetForwardVector() const
		{
			return Vec3F{
				Transform.m8,
				Transform.m9,
				Transform.m10
			};
		}

		Vec3F GetLeftVector() const
		{
			return Vec3F{
				-Transform.m0,
				-Transform.m1,
				-Transform.m2
			};
		}

		Vec3F GetDownVector() const
		{
			return Vec3F{
				-Transform.m4,
				-Transform.m5,
				-Transform.m6
			};
		}

		Vec3F GetBackwardVector() const
		{
			return Vec3F{
				-Transform.m8,
				-Transform.m9,
				-Transform.m10
			};
		}

		void Translate(const Vec3F& p_translation)
		{
			Position += p_translation;
		}

		void UpdateTransform()
		{
			Transform = Mat4F::ConstructTRSMatrix(Position, Rotation, Scale);
		}
	};

}

