#pragma once
#include "pchheader.h"

#include "ECSTypes.h"

#include "Component.h"

#include <set>

namespace Horyzen::ECS {

	class HORYZEN_OBJECT ComponentPool
	{
	public:

		ComponentPool(ComponentIDType p_ComponentID);
		~ComponentPool();

		void* GetComponent(u64 p_index);

		u64 AddComponent();
		void RemoveComponent(u64 p_index);

		u64 GetComponentsCount() const;

	private:

		u8* m_Data{ nullptr };
		u64 m_PoolSize{ 0 };

		ComponentIDType m_ComponentID;
		u64 m_ComponentSize;

		u64 m_InsertionIndex = 0;

		std::set<u64> m_IndexHoles;
		u64 m_MaxHoleIndex = 0;

	};

}