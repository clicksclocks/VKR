#include "pchheader.h"
#include "OpenGLTexture.h"

#include <glad/glad.h>


namespace Horyzen {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& p_filePath)
		: m_FilePath(p_filePath)
	{

	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{

	}

	u32 OpenGLTexture2D::GetWidth() const
	{
		return m_Width;
	}

	u32 OpenGLTexture2D::GetHeight() const
	{
		return m_Height;
	}

	void OpenGLTexture2D::Bind(u32 p_location) const
	{
		// OpenGL 4.5+
		glBindTextureUnit(p_location, m_ID);
	}

}