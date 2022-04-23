#include "pchheader.h"
#include "GPointCollection.h"

namespace Horyzen::Geo {

	GPointCollection::GPointCollection()
		: m_BufferSize(10000), m_DrawCount(0)
	{
		ResizeBuffer(10000);
	}

	u64 GPointCollection::AddPoint(Vec4D* p_coords, GObject* p_gObject, Color::RGBu p_color)
	{
		if (m_DrawCount == m_BufferSize) {
			ResizeBuffer(m_BufferSize * 2);
			m_BufferSize *= 2;
		}
		GPoint point(this);

		point.m_ID = SelectIDForPoint();
		point.m_Color = p_color;
		point.m_ColorID = GPoint::IDToColorID(point.m_ID);
		point.AddReference({ p_gObject , p_coords });
		m_GPointStorage.push_back(point);
		if (point.m_ID - 100 == m_IDToStorageIndex.size()) {
			m_IDToStorageIndex.push_back(m_GPointStorage.size() - 1);
		} else {
			m_IDToStorageIndex[point.m_ID - 100] = m_GPointStorage.size() - 1;
		}
		UpdatePointInBuffer(point.m_ID);
		m_DrawCount++;
		m_VertexArray->GetIndexBuffer()->SetCount(m_DrawCount);
		return point.m_ID;
	}

	void GPointCollection::DeletePoint(u64 p_ID)
	{
		u64 storageIndex = m_IDToStorageIndex[p_ID - 100];
		if (storageIndex != m_GPointStorage.size() - 1) {
			m_GPointStorage[storageIndex] = m_GPointStorage[m_GPointStorage.size() - 1];
		} 
		m_GPointStorage.erase(m_GPointStorage.end() - 1, m_GPointStorage.end());
		m_DrawCount--;
		if (p_ID != m_NextID - 1) {
			m_HolesInIDs.insert(p_ID);
		} else {
			m_NextID--;
		}
		m_VertexArray->GetIndexBuffer()->SetCount(m_DrawCount);
	}

	void GPointCollection::ResizeBuffer(u64 p_newSize)
	{
		if (m_VertexArray != nullptr) {
			m_VertexArray.reset();
		}
		m_VertexArray = VertexArray::Create();

		u64 newByteSize = p_newSize * sizeof(f32) * (3 + 3 + 3);

		auto vertexBuffer = VertexBuffer::Create(newByteSize);

		VertexBufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Color"    },
			{ ShaderDataType::Float3, "a_ColorID"  }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		std::vector<u32> indices(p_newSize);
		for (size_t i = 0; i < indices.size(); ++i) {
			indices[i] = i;
		}
		auto indexBuffer = IndexBuffer::Create(indices.data(), p_newSize);
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	u64 GPointCollection::SelectIDForPoint()
	{
		u64 ID;
		if (!m_HolesInIDs.empty()) {
			ID = *m_HolesInIDs.begin();
			m_HolesInIDs.erase(ID);	
		} else {
			ID = m_NextID;
			m_NextID++;
		}
		return ID;
	}

	void GPointCollection::UpdatePointInBuffer(u64 p_ID)
	{
		auto storageIndex = m_IDToStorageIndex[p_ID-100];
		const GPoint& point = m_GPointStorage[storageIndex];
		Vec4D& coords = *point.m_References[0].second;

		f32 data[9] = { static_cast<f32>(coords.x),
			            static_cast<f32>(coords.y),
			            static_cast<f32>(coords.z),
						point.m_Color.r / 255.0f,
			            point.m_Color.g / 255.0f,
			            point.m_Color.b / 255.0f,
						point.m_ColorID.r / 255.0f,
			            point.m_ColorID.g / 255.0f,
			            point.m_ColorID.b / 255.0f
		};

		u64 sizeOfOneEntry = 9 * sizeof(f32);

		auto vertexBuffer = m_VertexArray->GetVertexBuffers()[0];
		vertexBuffer->UpdateSubData(sizeOfOneEntry * storageIndex, sizeOfOneEntry, data);
	}

}

