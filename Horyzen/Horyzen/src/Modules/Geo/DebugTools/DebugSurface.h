#pragma once

#include "Horyzen/Math/Vec4.h"
#include "Horyzen/Core.h"

namespace Horyzen {
	class VertexArray;
}

namespace Horyzen::Geo {

	class DebugSurface
	{
	public:

		DebugSurface(const std::vector<Vec4D>& p_points, u64 p_tPointCount, u64 p_sPointCount, u64 p_maxLevels, u64 p_multiplier);
		
		void Draw();

	private:

		void BuildVertexArray();

	private:

		std::shared_ptr<VertexArray> m_VertexArray;

	};

}

