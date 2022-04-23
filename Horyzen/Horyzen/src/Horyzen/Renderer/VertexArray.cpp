#include "pchheader.h"
#include "VertexArray.h"

#include "Horyzen/Logger.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"



namespace Horyzen {

	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLVertexArray>();
			}
			default:
			{
				HORYZEN_LOG_ERROR("Current platform is not supported now!");
				return  nullptr;
			}
		}
	}
}