#pragma once
#include "../EntityComponentSystem/Registry.h"
#include "Entity.h"

namespace Horyzen {

//	class Entity;
	class ECS::System;
	class HORYZEN_OBJECT Scene
	{
	public:

		Scene();
		~Scene();

		Entity AddEntity();
		void RemoveEntity(ECS::EntityIDType p_entity);

		template <typename ComponentType_>
		ComponentType_& GetComponent(ECS::EntityIDType p_entity);

		template <typename ComponentType_>
		void RemoveComponent(ECS::EntityIDType p_entity);

		void UpdateSystem(f32 p_dt, ECS::System& p_system);

	private:

		ECS::Registry m_ECSRegistry;

		friend class Entity;

	};

	template<typename ComponentType_>
	inline ComponentType_& Scene::GetComponent(ECS::EntityIDType p_entityID)
	{
		return m_ECSRegistry.GetComponent<ComponentType_>(p_entityID);
	}

	template<typename ComponentType_>
	inline void Scene::RemoveComponent(ECS::EntityIDType p_entityID)
	{
		m_ECSRegistry.RemoveComponent<ComponentType_>(p_entityID);
	}

}

