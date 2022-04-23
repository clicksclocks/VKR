#include "pchheader.h"
#include "System.h"

namespace Horyzen::ECS {

	void System::AddComponentType(ComponentIDType p_componentType)
	{
		m_ComponentTypes.push_back(p_componentType);
	}
	const std::vector<ComponentIDType>& System::GetComponentTypes() const
	{
		return m_ComponentTypes;
	}

}
