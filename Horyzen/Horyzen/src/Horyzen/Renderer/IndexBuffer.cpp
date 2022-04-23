#include "pchheader.h"
#include "IndexBuffer.h"

#include "Horyzen/Logger.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Horyzen {

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(u32* p_indices, u32 p_count)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLIndexBuffer>(p_indices, p_count);
			}
			default:
			{
				HORYZEN_LOG_ERROR("Current platform is not supported now!");
				return  nullptr;
			}
		}
	}

}