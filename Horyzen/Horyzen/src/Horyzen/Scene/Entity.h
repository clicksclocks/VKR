#pragma once

#include "../EntityComponentSystem/ECSTypes.h"


namespace Horyzen {

	class Scene;
	class HORYZEN_OBJECT Entity
	{
	public:

		Entity(Scene& p_scene);
		~Entity();

		ECS::EntityIDType GetEntityID() const;

		template <typename ComponentType_>
		ComponentType_& AddComponent();

		template <typename ComponentType_>
		ComponentType_& AddComponent(ComponentType_&& p_component);

		template <typename ComponentType_>
		ComponentType_& GetComponent();


		template <typename ComponentType_>
		void RemoveComponent();

		template <typename ComponentType_>
		bool HasComponent();

	private:

		ECS::EntityIDType m_ID;
		Scene& m_SceneRef;

	};

	template<typename ComponentType_>
	inline ComponentType_& Entity::AddComponent()
	{
		return m_SceneRef.m_ECSRegistry.AddComponent<ComponentType_>(m_ID);

	}

	template<typename ComponentType_>
	inline ComponentType_& Entity::AddComponent(ComponentType_&& p_component)
	{
		// TODO: вставьте здесь оператор return
	}

	template<typename ComponentType_>
	inline ComponentType_& Entity::GetComponent()
	{
		return m_SceneRef.m_ECSRegistry.GetComponent<ComponentType_>(m_ID);
	}

	template<typename ComponentType_>
	inline void Entity::RemoveComponent()
	{
		m_SceneRef.m_ECSRegistry.RemoveComponent<ComponentType_>(m_ID);
	}

	template<typename ComponentType_>
	inline bool Entity::HasComponent()
	{
		return m_SceneRef.m_ECSRegistry.HasComponent<ComponentType_>(m_ID);
	}

}

