#pragma once

#include "Horyzen/Renderer/Shader.h"

namespace Horyzen {

	class OpenGLShader : public Shader
	{
	public:

		OpenGLShader(const std::string& p_vertexShaderSource, const std::string& p_fragmentShaderSource);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetUniform1f(u32 p_uniformLocation, f32 value) const override;
		virtual void SetUniform2f(u32 p_uniformLocation, f32 values[2]) const override;
		virtual void SetUniform3f(u32 p_uniformLocation, f32 values[3]) const override;
		virtual void SetUniform4f(u32 p_uniformLocation, f32 values[4]) const override;
		virtual void SetUniformMatrix4f(u32 p_uniformLocation, const f32* values) const override;

	private:

		u32 m_ID = 0;

	};

}

