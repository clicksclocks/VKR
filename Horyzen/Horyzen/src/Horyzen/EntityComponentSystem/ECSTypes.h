#pragma once
#include "../Core.h"

#include <unordered_map>

namespace Horyzen::ECS {

	typedef u64 EntityIDType;
	typedef u64 ComponentIDType;

	typedef void* EntityHandle;

	struct EntityInternal
	{
		EntityIDType ID;
		std::unordered_map<ComponentIDType, u64> Components;
	};

}
