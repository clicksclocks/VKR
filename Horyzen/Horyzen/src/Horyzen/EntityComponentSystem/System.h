#pragma once
#include "Component.h"

#include <vector>

namespace Horyzen::ECS {

	class System
	{
	public:

		void AddComponentType(ComponentIDType p_componentType);
		const std::vector<ComponentIDType>& GetComponentTypes() const;

		virtual void UpdateComponents(f32 p_dt, ComponentBase** p_components) = 0;

	private:

		std::vector<ComponentIDType> m_ComponentTypes;

	};

}

