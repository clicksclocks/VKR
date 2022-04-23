#include "pchheader.h"
#include "DebugSurface.h"

#include "Horyzen/Renderer/VertexArray.h"
#include "Horyzen/Renderer/Renderer.h"

#include <limits>


#undef min
#undef max

namespace Horyzen::Geo {

	DebugSurface::DebugSurface(const std::vector<Vec4D>& p_points, u64 p_tPointCount, u64 p_sPointCount, u64 p_maxLevels, u64 p_multiplier)
	{
		f64 min = std::numeric_limits<f64>::max();
		f64 max = std::numeric_limits<f64>::min();
		

		for (size_t i = 0; i < p_points.size(); ++i) {
			if (p_points[i].w > max) max = p_points[i].w;
			if (p_points[i].w < min) min = p_points[i].w;
		}
		f64 mid = (min + max) * 0.5;
		f64 range = max - min;
		mid = 0;

		std::vector<f32> data(p_points.size() * 3 * 2);

		size_t ind = 0;
		for (size_t i = 0; i < data.size(); i += 6) {
			data[i]   = static_cast<f32>(p_points[ind].x);
			data[i+1] = static_cast<f32>(p_points[ind].y);
			data[i+2] = static_cast<f32>(p_points[ind].z);

			f32 color[3];
			f64 value = p_points[ind].w;
			//value = -value;
			//max = -min;

			max = p_maxLevels;
			if (value < mid) {
			/*	u64 multiplier = 1;

				value = std::abs(value);
				f32 maxHere = std::ceil(std::abs(min));
				value = value / maxHere;

				f32 step = 1.0 /(multiplier * maxHere);

				u64 count = static_cast<u64>(multiplier * maxHere);
				f32 r = 0, g = 0, b = 0;
				for (size_t i = 0; i <= count - 1; ++i) {
					f32 cLow  = i * step;
					f32 cHigh = (i+1) * step;
					if (cLow <= value && value <= cHigh) {
						g = 1.0 - cLow;
						b = 0.0;
						r = cLow;
						if (i == 0) b = 0.4;
					}
				}

				data[i+3] = r;
				data[i+4] = g;
				data[i+5] = b;*/


			} else {
				u64 multiplier = p_multiplier;

				value = std::abs(value);
				f32 maxHere = std::ceil(max);
				value = 2 * value / maxHere;

				//f32 step = 1.0 /(multiplier * maxHere);

				//u64 count = static_cast<u64>(multiplier *maxHere);
				//f32 r = 0, g = 0, b = 0;
				//for (size_t i = 0; i <= count - 1; ++i) {
				//	f32 cLow = i * step;
				//	f32 cHigh = (i + 1) * step;
				//	if (cLow <= value && value <= cHigh) {
				//		g = 1.0 - cLow;
				//		b = cLow;
				//		r = 0.0;
				//		if (i == 0) r = 0.3;
				//	}
				//}

				f32 step = 2.0 / (multiplier * maxHere);

				u64 count = static_cast<u64>(multiplier * maxHere);
				f32 r = 0, g = 0, b = 0;
				for (size_t i = 0; i <= count - 1; ++i) {
					f32 cLow = i * step;
					f32 cHigh = (i + 1) * step;
					if (cLow <= value && value <= cHigh) {
						if (cLow < 1.0) {
							g = cLow;
							b = 1.0 - cLow;
							r = 0.0;
						} else {
							cLow -= 1.0;
							cHigh -= 1.0;
							g = 1.0 - cHigh;
							b = 0;
							r = cHigh;
						}
					}
				}

				data[i+3] = r;
				data[i+4] = g;
				data[i+5] = b;


			}
			ind++;
		}

		u64 byteSize = p_tPointCount * p_sPointCount * 6 * sizeof(f32);
		auto vertexBuffer = VertexBuffer::Create(data.data(), byteSize);
		VertexBufferLayout layout =
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);

		std::vector<u32> indices;

		for (size_t i = 0; i < p_tPointCount - 1; ++i) {
			indices.push_back(i * p_sPointCount);
			for (size_t j = 0; j < p_sPointCount - 1; ++j) {
//				indices.push_back(i * p_sPointCount + j);
				indices.push_back((i + 1) * p_sPointCount + j);
				indices.push_back(i * p_sPointCount + j + 1);
			}
			indices.push_back((i + 1) * p_sPointCount + p_tPointCount - 1);
			indices.push_back((i + 1) * p_sPointCount + p_tPointCount - 1);
			indices.push_back((i + 1) * p_sPointCount);
			indices.push_back((i + 1) * p_sPointCount);
		}

		//indices = { 0, 3, 1, 4, 2, 5, 5, 8, 4, 7, 3, 6 };

		auto indexBuffer = IndexBuffer::Create(indices.data(), indices.size());

		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}

	void DebugSurface::Draw()
	{
		Renderer::Submit(m_VertexArray, BasePrimitive::TriangleStrip);
	}

	void DebugSurface::BuildVertexArray()
	{
	}

}
