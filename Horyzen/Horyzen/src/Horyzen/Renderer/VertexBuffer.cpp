#include "pchheader.h"
#include "VertexBuffer.h"
#include "Horyzen/Logger.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace Horyzen {

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(f32* p_vertices, u32 p_byteSize)
    {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLVertexBuffer>(p_vertices, p_byteSize);
			}
			default:
			{
				HORYZEN_LOG_ERROR("Current platform is not supported now!");
				return  nullptr;
			}
		}
    }

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(u32 p_byteSize)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLVertexBuffer>(p_byteSize);
			}
			default:
			{
				HORYZEN_LOG_ERROR("Current platform is not supported now!");
				return  nullptr;
			}
		}
	}

}