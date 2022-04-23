#include "pchheader.h"
#include "ComponentPool.h"

#include "../Logger.h"


namespace Horyzen::ECS {

	ComponentPool::ComponentPool(ComponentIDType p_ComponentID)
		: m_Data(new u8[1000 * ComponentBase::GetComponentSize(p_ComponentID)]),
		  m_PoolSize(1000),
		  m_ComponentID(p_ComponentID),
		  m_ComponentSize(ComponentBase::GetComponentSize(p_ComponentID))
	{}

	ComponentPool::~ComponentPool()
	{
		delete[] m_Data;
	}

	void* ComponentPool::GetComponent(u64 p_index)
	{
		return &m_Data[p_index * m_ComponentSize];
	}

	inline u64 ComponentPool::AddComponent()
	{
		auto createFn = ComponentBase::GetComponentCreateFunction(m_ComponentID);
		createFn(&m_Data[m_InsertionIndex * m_ComponentSize]);
		return m_InsertionIndex++;
	}

	inline void ComponentPool::RemoveComponent(u64 p_index)
	{
		HORYZEN_ASSERT(p_index < m_InsertionIndex,
			"Deleting component with memory index {0}, but insetrion index is only {1}!",
			p_index, m_InsertionIndex
		);

		auto freeFn = ComponentBase::GetComponentCreateFunction(m_ComponentID);
		freeFn(&m_Data[p_index * m_ComponentSize]);


		if (p_index != m_InsertionIndex - 1) {
			ComponentBase* base = reinterpret_cast<ComponentBase*>(&m_Data[(m_InsertionIndex - 1) * m_ComponentSize]);
			auto entity = (EntityInternal*) base->GetEntityHandle();

			auto moveFn = ComponentBase::GetComponentMoveFunction(m_ComponentID);
			moveFn(&m_Data[p_index * m_ComponentSize], &m_Data[(m_InsertionIndex - 1) * m_ComponentSize]);

			entity->Components[m_ComponentID] = p_index;
		}
		m_InsertionIndex--;
	}

	u64 ComponentPool::GetComponentsCount() const
	{
		return m_InsertionIndex - m_IndexHoles.size();
	}

}