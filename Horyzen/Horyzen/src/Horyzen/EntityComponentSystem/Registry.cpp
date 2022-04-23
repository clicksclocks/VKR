#pragma once
#include "pchheader.h"
#include "Registry.h"

#include "Horyzen/Logger.h"

namespace Horyzen::ECS {

	Registry::Registry()
		: m_ComponentPools(ComponentBase::GetMaxID(), nullptr)
	{
		HORYZEN_LOG_ERROR("Max component ID: {0}", ComponentBase::GetMaxID());
	}

	Registry::~Registry()
	{
	}

	EntityIDType Registry::CreateEntity()
	{
		EntityInternal* newEntity = new EntityInternal();
		newEntity->ID = m_Entities.size();
		m_Entities.push_back(newEntity);

		HORYZEN_LOG_ERROR("New entity! ID: {0}", newEntity->ID);
		return newEntity->ID;
	}

	void Registry::RemoveEntity(EntityIDType p_entityID)
	{
		HORYZEN_ASSERT(p_entityID < m_Entities.size(), "Entity with ID {0} doesn't exists", p_entityID);

		EntityInternal* entity = m_Entities[p_entityID];
		for (auto compKey : entity->Components) {
			ComponentIDType compID = compKey.first;
			u64 index = compKey.second;
			ComponentPool* pool = m_ComponentPools[compID];
			pool->RemoveComponent(index);
		}
		delete entity;
	}

	void Registry::UpdateSystem(f32 p_dt, System& p_system)
	{
		auto componentTypes = p_system.GetComponentTypes();
		if (componentTypes.size() == 1) {
			// Переделать эту ересь
			auto componentID = componentTypes[0];
			ComponentPool* pool = m_ComponentPools[componentID];
			auto componentsCount = pool->GetComponentsCount();
			for (size_t i = 0; i < componentsCount; ++i) {
				ComponentBase* component = (ComponentBase*)pool->GetComponent(i);
				p_system.UpdateComponents(p_dt, &component);
			}
		} else {
			UpdateSystemWithMultipleComponents(p_dt, p_system);
		}

	}

	void Registry::UpdateSystemWithMultipleComponents(f32 p_dt, System& p_system)
	{
		auto componentTypes = p_system.GetComponentTypes();

		// Search for least common component
		u64 leastCommonCount = (u64)-1;
		u64 leastCommonIndex = (u64)-1;
		for (size_t i = 0; i < componentTypes.size(); ++i) {
			auto componentID = componentTypes[i];
			u64 count = m_ComponentPools[componentID]->GetComponentsCount();
			if (count < leastCommonCount) {
				leastCommonCount = count;
				leastCommonIndex = i;
			}
		}
		ComponentIDType leastCommonID = componentTypes[leastCommonIndex];
		ComponentPool* leastCommonPool = m_ComponentPools[leastCommonID];

		std::vector<ComponentBase*> componentsGathered(componentTypes.size(), nullptr);

		for (size_t i = 0; i < leastCommonCount; ++i) {
			ComponentBase* leastComponent = (ComponentBase*)leastCommonPool->GetComponent(i);
			componentsGathered[leastCommonIndex] = leastComponent;

			auto entity = (EntityInternal*)leastComponent->GetEntityHandle();

			bool success = true;
			for (size_t k = 0; k < componentTypes.size(); ++k) {
				ComponentIDType compID = componentTypes[k];
				if (compID == leastCommonID) {
					continue;
				}

				auto iter = entity->Components.find(compID);
				if (iter != entity->Components.end()) {
					u64 index = iter->second;
					ComponentPool* currentPool = m_ComponentPools[compID];
					ComponentBase* currentComponent = (ComponentBase*)currentPool->GetComponent(index);
					componentsGathered[k] = currentComponent;
				}
				else {
					success = false;
					break;
				}
			}
			if (success) {
				p_system.UpdateComponents(p_dt, componentsGathered.data());
			}
			
		}
	}

}