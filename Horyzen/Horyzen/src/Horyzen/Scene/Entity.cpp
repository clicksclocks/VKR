#include "pchheader.h"
#include "Entity.h"

#include "Scene.h"

namespace Horyzen {

	Entity::Entity(Scene& p_scene)
		: m_SceneRef(p_scene)
	{
		m_ID = p_scene.m_ECSRegistry.CreateEntity();
	}

	Entity::~Entity()
	{
	}

	ECS::EntityIDType Entity::GetEntityID() const
	{
		return m_ID;
	}

}