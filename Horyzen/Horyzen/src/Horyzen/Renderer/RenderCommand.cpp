#include "pchheader.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Horyzen {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}