#pragma once

#include "Horyzen/Renderer/Texture.h"

namespace Horyzen {

	class OpenGLTexture2D : public Texture2D
	{
	public:

		OpenGLTexture2D(const std::string& p_filePath);
		virtual ~OpenGLTexture2D();

		virtual u32 GetWidth() const override;
		virtual u32 GetHeight() const override;

		virtual void Bind(u32 p_location) const override;

	private:

		std::string m_FilePath;
		u32 m_ID;
		u32 m_Width;
		u32 m_Height;

	};

}