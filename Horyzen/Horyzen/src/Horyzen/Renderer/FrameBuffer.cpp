#include "pchheader.h"
#include "FrameBuffer.h"

#include "Horyzen/Logger.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Horyzen {

	std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& p_properties)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLFrameBuffer>(p_properties);
			}
			default:
			{
				HORYZEN_LOG_ERROR("Current platform is not supported now!");
				return  nullptr;
			}
		}
	}

	void FrameBuffer::Blit(std::shared_ptr<FrameBuffer> p_destinationFBO,
		                   u64                          p_destinationAttachmentIndex,
		                   std::shared_ptr<FrameBuffer> p_sourceFBO,
		                   u64                          p_sourceAttachmentIndex)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
			{
				p_sourceFBO->Blit(p_sourceAttachmentIndex,
					              p_destinationFBO,
					              p_destinationAttachmentIndex);
			} break;
			default:
			{
				HORYZEN_LOG_ERROR("Current platform is not supported now!");
			}
		}
	}

}