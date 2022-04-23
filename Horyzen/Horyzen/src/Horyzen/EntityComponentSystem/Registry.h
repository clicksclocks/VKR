#pragma once
#include "ECSTypes.h"
#include "ComponentPool.h"
#include "Component.h"
#include "System.h"

#include <vector>


namespace Horyzen::ECS {

	class Registry
	{
	public:

		Registry();
		~Registry();

		EntityIDType CreateEntity();
		void RemoveEntity(EntityIDType p_entityID);

		template <typename ComponentType_>
		ComponentType_& AddComponent(EntityIDType p_entityID);

		template <typename ComponentType_>
		ComponentType_& AddComponent(EntityIDType p_entityID, ComponentType_&& p_component);

		template <typename ComponentType_>
		void RemoveComponent(EntityIDType p_entityID);

		template <typename ComponentType_>
		bool HasComponent(EntityIDType p_entityID);

		template <typename ComponentType_>
		ComponentType_& GetComponent(ECS::EntityIDType p_entityID);

		void UpdateSystem(f32 p_dt, System& p_system);

	private:

		void UpdateSystemWithMultipleComponents(f32 p_dt, System& p_system);

	private:

		std::vector<EntityInternal*> m_Entities;
		std::vector<ComponentPool*> m_ComponentPools;

	};

	template<typename ComponentType_>
	inline ComponentType_& Registry::AddComponent(EntityIDType p_entityID)
	{
		if (m_ComponentPools[ComponentType_::GetComponentID()] == nullptr) {
			m_ComponentPools[ComponentType_::GetComponentID()] 
				= new ComponentPool(ComponentType_::GetComponentID());
		}
		ComponentPool* pool = m_ComponentPools[ComponentType_::GetComponentID()];
		u64 index = pool->AddComponent();

		EntityInternal* entity = m_Entities[p_entityID];
		entity->Components[ComponentType_::GetComponentID()] = index;

		ComponentType_& component = *static_cast<ComponentType_*>(pool->GetComponent(index));
		component.SetEntityHandle((void*)(entity));

		//HORYZEN_LOG_TRACE("To entity with ID {0} adding component with ID {1}. Memory index {2}",
		//	entity->ID, ComponentType_::GetComponentID(), index
		//);

		return component;
	}

	template<typename ComponentType_>
	inline ComponentType_& Registry::AddComponent(EntityIDType p_entityID, ComponentType_&& p_component)
	{
		// TODO: вставьте здесь оператор return
	}

	template<typename ComponentType_>
	inline void Registry::RemoveComponent(EntityIDType p_entityID)
	{
		HORYZEN_ASSERT(p_entityID < m_Entities.size(), "Entity with ID {0} doesn't exists", p_entityID);

		EntityInternal* entity = m_Entities[p_entityID];
		HORYZEN_ASSERT(entity->Components.find(ComponentType_::GetComponentID()) != entity->Components.end(),
			"Entity with ID {0} doesn't have a component with ID {1}",
			p_entityID, ComponentType_::GetComponentID()
		);

		u64 index = entity->Components[ComponentType_::GetComponentID()];

		ComponentPool* pool = m_ComponentPools[ComponentType_::GetComponentID()];
		pool->RemoveComponent(index);
		entity->Components.erase(ComponentType_::GetComponentID());
	}

	template<typename ComponentType_>
	inline bool Registry::HasComponent(EntityIDType p_entityID)
	{
		HORYZEN_ASSERT(p_entityID < m_Entities.size(), "Entity with ID {0} doesn't exists", p_entityID);
		EntityInternal* entity = m_Entities[p_entityID];
		auto iter = entity->Components.find(ComponentType_::GetComponentID());
		if (iter != entity->Components.end()) {
			return true;
		} else {
			return false;
		}
	}

	template<typename ComponentType_>
	inline ComponentType_& Registry::GetComponent(ECS::EntityIDType p_entityID)
	{
		HORYZEN_ASSERT(p_entityID < m_Entities.size(), "Entity with ID {0} doesn't exists", p_entityID);

		EntityInternal* entity = m_Entities[p_entityID];
		HORYZEN_ASSERT(entity->Components.find(ComponentType_::GetComponentID()) != entity->Components.end(),
			"Entity with ID {0} doesn't have a component with ID {1}",
			p_entityID, ComponentType_::GetComponentID()
		);

		u64 index = entity->Components[ComponentType_::GetComponentID()];

		//HORYZEN_LOG_TRACE("From entity with ID {0} getting component with ID {1}. Memory index {2}",
		//	entity->ID, ComponentType_::GetComponentID(), index
		//);

		ComponentPool* pool = m_ComponentPools[ComponentType_::GetComponentID()];
		return *static_cast<ComponentType_*>(pool->GetComponent(index));
	}

}