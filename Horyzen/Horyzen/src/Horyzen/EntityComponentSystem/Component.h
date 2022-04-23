#pragma once
#include "ECSTypes.h"


#include<functional>

namespace Horyzen::ECS {

	using ComponentCreateFunctionType = std::function<void(void*)>;
	using ComponentFreeFunctionType   = std::function<void(void*)>;
	using ComponentMoveFunctionType   = std::function<void(void*, void*)>;

	struct ComponentInfo
	{
		ComponentCreateFunctionType CreateFunc;
		ComponentFreeFunctionType   FreeFunc;
		ComponentMoveFunctionType   MoveFunc;
		size_t Size{ 0 };
	};

	class HORYZEN_OBJECT ComponentBase
	{
	public:

		static ComponentIDType RegisterComponentType(
			ComponentCreateFunctionType p_createFunc,
			ComponentFreeFunctionType p_freeFunc,
			ComponentMoveFunctionType p_moveFunc,
			size_t size);

		static ComponentIDType GetMaxID();

		static ComponentCreateFunctionType& GetComponentCreateFunction(ComponentIDType p_componentID);
		static ComponentFreeFunctionType& GetComponentFreeFunction(ComponentIDType p_componentID);
		static ComponentMoveFunctionType& GetComponentMoveFunction(ComponentIDType p_componentID);
		static u64 GetComponentSize(ComponentIDType p_componentID);

		void SetEntityHandle(EntityHandle p_handle);
		EntityHandle GetEntityHandle() const;

	private:

		struct ComponentInfoContainer 
		{

			~ComponentInfoContainer()
			{
				delete[] Data;
			}

			void InitContainer()
			{
				Data = new ComponentInfo[100];
				Size = 0;
				Capacity = 100;
			}

			void AddEntry(const ComponentInfo& p_entry)
			{
				if (Size == Capacity) {
					Capacity *= 2;
					ComponentInfo* newMemory = new ComponentInfo[Capacity];
					for (size_t i = 0; i < Size; ++i) {
						newMemory[i] = Data[i];
					}
					delete[] Data;
					Data = newMemory;
				}
				Data[Size] = p_entry;
				Size++;
			}

			ComponentInfo* Data = nullptr;
			u64 Size = 0;
			u64 Capacity = 0;

		};

		EntityHandle m_EntityHandle;

	private:

		static ComponentIDType s_NextID;
		static ComponentInfoContainer s_ComponentInfos;

	};

	/// <summary>
	/// This class is the intermediate class between custom Component class
	/// and ComponentBase class. It stores the essential information about
	/// component's size, it's Create and Free functions. The point is that
	/// this information is accessable through an object of a custom component
	/// type.
	/// </summary>
	/// <typeparam name="ComponentType_"></typeparam>
	template <typename ComponentType_>
	class Component : public ComponentBase
	{
	public:

		static ComponentIDType GetComponentID();
		static ComponentIDType GetComponentSize();

	private:

		static const ComponentIDType s_ComponentID;

		static const size_t s_ComponentSize;
		static const ComponentCreateFunctionType CreateFunc;
		static const ComponentFreeFunctionType FreeFunc;
		static const ComponentFreeFunctionType MoveFunc;

	};

	template <typename ComponentType_>
	const ComponentIDType Component<ComponentType_>::s_ComponentSize = sizeof(ComponentType_);


	template <typename ComponentType_>
	const void ComponentCreateFunc(void* p_adress)
	{
		new(p_adress) ComponentType_();
	}

	template <typename ComponentType_>
	const void ComponentFreeFunc(void* p_adress)
	{
		(*(ComponentType_*)p_adress).~ComponentType_();
	}

	template <typename ComponentType_>
	const void ComponentMoveFunc(void* p_destinationAdress, void* p_sourceAdress)
	{
		ComponentType_* comp = static_cast<ComponentType_*>(p_sourceAdress);
		new(p_destinationAdress) ComponentType_(std::move(*comp));
	}

	template <typename ComponentType_>
	const ComponentCreateFunctionType Component<ComponentType_>::CreateFunc = ComponentCreateFunc<ComponentType_>;
	

	template <typename ComponentType_>
	const ComponentFreeFunctionType Component<ComponentType_>::FreeFunc = ComponentFreeFunc<ComponentType_>;

	template <typename ComponentType_>
	const ComponentIDType Component<ComponentType_>::s_ComponentID = ComponentBase::RegisterComponentType(
		ComponentCreateFunc<ComponentType_>,
		ComponentFreeFunc<ComponentType_>,
		ComponentMoveFunc<ComponentType_>,
		sizeof(ComponentType_));


	template <typename ComponentType_>
	ComponentIDType Component<ComponentType_>::GetComponentID()
	{
		return s_ComponentID;
	}

	template <typename ComponentType_>
	ComponentIDType Component<ComponentType_>::GetComponentSize()
	{
		return s_ComponentSize;
	}

}