#include "pchheader.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Horyzen {

	static GLenum HoryzenPrimitiveToOpenGLPrimitive(BasePrimitive p_primitive)
	{
		switch (p_primitive)
		{
			case BasePrimitive::Points:        return GL_POINTS;
			case BasePrimitive::Lines:         return GL_LINES;
			case BasePrimitive::LineStrip:     return GL_LINE_STRIP;
			case BasePrimitive::Triangles:     return GL_TRIANGLES;
			case BasePrimitive::TriangleStrip: return GL_TRIANGLE_STRIP;
		}
	}

	void OpenGLRendererAPI::SetClearColor(f32 p_r, f32 p_g, f32 p_b, f32 p_a)
	{
		glClearColor(p_r, p_g, p_b, p_a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& p_vertexArray, BasePrimitive p_primitive)
	{	
		glDrawElements(
			HoryzenPrimitiveToOpenGLPrimitive(p_primitive),
			p_vertexArray->GetIndexBuffer()->GetCount(), 
			GL_UNSIGNED_INT, 
			nullptr);
	}

	void OpenGLRendererAPI::ReadPixelFromScreenBuffer(u32 x, u32 y, void* data)
	{
		glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, data);
	}

}