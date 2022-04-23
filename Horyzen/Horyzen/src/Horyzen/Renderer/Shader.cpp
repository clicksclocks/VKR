#include "pchheader.h"
#include "Shader.h"

#include "Horyzen/Logger.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"


namespace  Horyzen {

	std::shared_ptr<Shader> Shader::Create(const std::string& p_vertexShaderSource,
		                                   const std::string& p_fragmentShaderSource)
    {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLShader>(p_vertexShaderSource, p_fragmentShaderSource);
			}
			default:
			{
				HORYZEN_LOG_ERROR("Current platform is not supported now!");
				return  nullptr;
			}
		}
	}
    
}