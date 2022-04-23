#pragma once
#include "GPoint.h"
#include "Horyzen/Renderer/VertexArray.h"

namespace Horyzen::Geo {

	class GPointCollection
	{
	public:

		friend class GeoRenderSystem;
		friend class GPointPickingSystem;
		friend class GObject;

		GPointCollection();

		u64 AddPoint(Vec4D* p_coords, GObject* p_gObject, Color::RGBu p_color);
		void DeletePoint(u64 p_ID);

	private:

		void ResizeBuffer(u64 p_newSize);
		u64 SelectIDForPoint();
		void UpdatePointInBuffer(u64 p_ID);

	private:

		std::vector<GPoint> m_GPointStorage;
		u64 m_NextID = 100;
		std::shared_ptr<VertexArray> m_VertexArray{ nullptr };
		std::vector<u64> m_IDToStorageIndex;
		std::set<u64> m_HolesInIDs;

		u64 m_BufferSize;
		u64 m_DrawCount;

	};

}



