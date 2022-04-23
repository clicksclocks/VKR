#include "pchheader.h"
#include "Scene.h"

#include "Entity.h"

namespace Horyzen {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::AddEntity()
	{
		return Entity(*this);
	}

	void Scene::RemoveEntity(ECS::EntityIDType p_entity)
	{
		m_ECSRegistry.RemoveEntity(p_entity);
	}

	void Scene::UpdateSystem(f32 p_dt, ECS::System& p_system)
	{
		m_ECSRegistry.UpdateSystem(p_dt, p_system);
	}

}