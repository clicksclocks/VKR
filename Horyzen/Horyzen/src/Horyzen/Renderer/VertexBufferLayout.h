#pragma once
#include "pchheader.h"
#include "Horyzen/Core.h"
#include "../Logger.h"


namespace Horyzen {

	enum class ShaderDataType
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	static u32 ShaderDataTypeSize(ShaderDataType p_type)
	{
		switch (p_type)
		{
			case ShaderDataType::Float:   return 4;
			case ShaderDataType::Float2:  return 4 * 2;
			case ShaderDataType::Float3:  return 4 * 3;
			case ShaderDataType::Float4:  return 4 * 4;
			case ShaderDataType::Mat3:    return 4 * 3 * 3;
			case ShaderDataType::Mat4:    return 4 * 4 * 4;
			case ShaderDataType::Int:     return 4;
			case ShaderDataType::Int2:    return 4 * 2;
			case ShaderDataType::Int3:    return 4 * 3;
			case ShaderDataType::Int4:    return 4 * 4;
			case ShaderDataType::Bool:    return 1;
			default:
			{
				HORYZEN_ASSERT(false, "Unknown ShaderDataType!");
				return 0;
			}
		}
	}

	struct HORYZEN_OBJECT VertexBufferElement
	{
		ShaderDataType Type;
		std::string Name;
		bool Normalized;
		u32 Size;
		u32 Offset;

		VertexBufferElement() = default;

		VertexBufferElement(ShaderDataType     p_type,
			                const std::string& p_name,
			                bool               p_normalized = false)
			: Type(p_type), Name(p_name), Normalized(p_normalized), Size(ShaderDataTypeSize(p_type)), Offset(0)
		{}

		u32 GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Float2:  return 2;
				case ShaderDataType::Float3:  return 3;
				case ShaderDataType::Float4:  return 4;
				case ShaderDataType::Mat3:    return 3 * 3;
				case ShaderDataType::Mat4:    return 4 * 4;
				case ShaderDataType::Int:     return 1;
				case ShaderDataType::Int2:    return 2;
				case ShaderDataType::Int3:    return 3;
				case ShaderDataType::Int4:    return 4;
				case ShaderDataType::Bool:    return 1;
				default:
				{
					HORYZEN_ASSERT(false, "Unknown ShaderDataType!");
					return 0;
				}
			}
		}
	};

	class HORYZEN_OBJECT VertexBufferLayout
	{
	public:

		VertexBufferLayout(const std::initializer_list<VertexBufferElement>& p_elements)
			: m_Elements(p_elements)
		{
			CalculateOffsetAndStride();
		}

		VertexBufferLayout() = default;

		inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
		inline u32 GetStride() const { return m_Stride; }

		inline std::vector<VertexBufferElement>::iterator begin() { return m_Elements.begin(); }
		inline std::vector<VertexBufferElement>::iterator end() { return m_Elements.end(); }

		inline std::vector<VertexBufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		inline std::vector<VertexBufferElement>::const_iterator end()	const { return m_Elements.end(); }

	private:

		void CalculateOffsetAndStride()
		{
			u32 offset = 0;
			m_Stride = 0;
			for (auto& elem : m_Elements) {
				elem.Offset = offset;
				offset += elem.Size;
				m_Stride += elem.Size;
			}
		}

	private:

		std::vector<VertexBufferElement> m_Elements;
		u32 m_Stride = 0;

	};



}