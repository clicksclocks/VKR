#include "pchheader.h"
#include "OpenGLRenderingContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Horyzen/Logger.h"

namespace Horyzen {

	void OpenGLErrorCallback(GLenum        source,
		                     GLenum        type,
		                     GLuint        id,
		                     GLenum        severity,
		                     GLsizei       length,
		                     GLchar const* message,
		                     void const*   user_param)
	{
		auto source_str = [source]() -> std::string {
			switch (source)
			{
				case GL_DEBUG_SOURCE_API:             return "API";
				case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "WINDOW SYSTEM";
				case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
				case GL_DEBUG_SOURCE_THIRD_PARTY:     return "THIRD PARTY";
				case GL_DEBUG_SOURCE_APPLICATION:     return "APPLICATION";
				case GL_DEBUG_SOURCE_OTHER:           return "OTHER";
				default: return "UNKNOWN";
			}
		}();

		auto type_str = [type]() {
			switch (type)
			{
				case GL_DEBUG_TYPE_ERROR:               return "ERROR";
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UNDEFINED_BEHAVIOR";
				case GL_DEBUG_TYPE_PORTABILITY:         return "PORTABILITY";
				case GL_DEBUG_TYPE_PERFORMANCE:         return "PERFORMANCE";
				case GL_DEBUG_TYPE_MARKER:              return "MARKER";
				case GL_DEBUG_TYPE_OTHER:               return "OTHER";
				default: return "UNKNOWN";
			}
		}();

		auto severity_str = [severity]() {
			switch (severity) {
				case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
				case GL_DEBUG_SEVERITY_LOW:          return "LOW";
				case GL_DEBUG_SEVERITY_MEDIUM:       return "MEDIUM";
				case GL_DEBUG_SEVERITY_HIGH:         return "HIGH";
				default: return "UNKNOWN";
			}
		}();

		if (type == GL_DEBUG_TYPE_OTHER) {
			return;
		}

		switch (severity) {

			case GL_DEBUG_SEVERITY_NOTIFICATION: {
				HORYZEN_LOG_INFO("{0}, {1}, {2}, {3}\n",
								  source_str,
								  type_str,
								  id,
								  message);
			} break;

			case GL_DEBUG_SEVERITY_LOW: {
				HORYZEN_LOG_WARN("{0}, {1}, {2}, {3}\n",
					             source_str,
					             type_str,
					             id,
					             message);
			} break;

			case GL_DEBUG_SEVERITY_MEDIUM: {
				HORYZEN_LOG_ERROR("{0}, {1}, {2}, {3}\n",
					              source_str,
					              type_str,
					              id,
					              message);
			} break;

			case GL_DEBUG_SEVERITY_HIGH: {
				HORYZEN_LOG_ERROR("{0}, {1}, {2}, {3}\n",
					              source_str,
					              type_str,
					              id,
					              message);
			}
		}

	}

	OpenGLRenderingContext::OpenGLRenderingContext(GLFWwindow* p_windowHandle)
		: m_GLFWWindowHandle(p_windowHandle)
	{}

	void OpenGLRenderingContext::Init()
	{
		glfwMakeContextCurrent(m_GLFWWindowHandle);
		i32 gladInitSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HORYZEN_ASSERT(gladInitSuccess, "Glad failed to initialize!");

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
	//	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
		glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(OpenGLErrorCallback, nullptr);


		// Getting OpenGL
		auto glVersion = glGetString(GL_VERSION);
		auto glVendor = glGetString(GL_VENDOR);
		auto glRenderer = glGetString(GL_RENDERER);
		HORYZEN_LOG_INFO(
			R"(OpenGL info:
    Version: {0}
    Vendor: {1}  
    Renderer: {2})",
			glVersion, glVendor, glRenderer);

		GLint maxSamples;
		glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
		HORYZEN_LOG_INFO("Maximum supported samples: {0}", maxSamples);

	}
	void OpenGLRenderingContext::SwapBuffers()
	{
		glfwSwapBuffers(m_GLFWWindowHandle);
	}
	void OpenGLRenderingContext::ViewPort(u32 p_x, u32 p_y, u32 p_width, u32 p_height)
	{
		glViewport(static_cast<GLint>(p_x),
			       static_cast<GLint>(p_y),
			       static_cast<GLint>(p_width),
			       static_cast<GLint>(p_height));
	}
}
