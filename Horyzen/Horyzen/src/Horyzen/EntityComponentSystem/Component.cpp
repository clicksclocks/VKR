#include "pchheader.h"
#include "Component.h"

namespace Horyzen::ECS {

	ComponentIDType ComponentBase::s_NextID = 0;
	ComponentBase::ComponentInfoContainer ComponentBase::s_ComponentInfos;

	ComponentIDType ComponentBase::RegisterComponentType(
		ComponentCreateFunctionType p_createFunc,
		ComponentFreeFunctionType p_freeFunc,
		ComponentMoveFunctionType p_moveFunc,
		size_t p_size)
	{
		if (s_ComponentInfos.Data == nullptr) {
			s_ComponentInfos.InitContainer();
		}
		s_ComponentInfos.AddEntry({ p_createFunc, p_freeFunc, p_moveFunc, p_size });
		return s_NextID++;
	}

	ComponentIDType ComponentBase::GetMaxID()
	{
		return s_NextID;
	}

	ComponentCreateFunctionType& ComponentBase::GetComponentCreateFunction(ComponentIDType p_componentID)
	{
		return s_ComponentInfos.Data[p_componentID].CreateFunc;
	}

	ComponentFreeFunctionType& ComponentBase::GetComponentFreeFunction(ComponentIDType p_componentID)
	{
		return s_ComponentInfos.Data[p_componentID].FreeFunc;
	}

	ComponentMoveFunctionType& ComponentBase::GetComponentMoveFunction(ComponentIDType p_componentID)
	{
		return s_ComponentInfos.Data[p_componentID].MoveFunc;
	}

	u64 ComponentBase::GetComponentSize(ComponentIDType p_componentID)
	{
		return s_ComponentInfos.Data[p_componentID].Size;
	}

	void ComponentBase::SetEntityHandle(EntityHandle p_handle)
	{
		m_EntityHandle = p_handle;
	}

	EntityHandle ComponentBase::GetEntityHandle() const
	{
		return m_EntityHandle;
	}



}