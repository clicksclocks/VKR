#pragma once

#include "pchheader.h"
#include "Horyzen/Core.h"

namespace Horyzen {

	class HORYZEN_OBJECT Shader
	{
	public:

		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetUniform1f(u32 p_uniformLocation, f32 value) const = 0;
		virtual void SetUniform2f(u32 p_uniformLocation, f32 values[2]) const = 0;
		virtual void SetUniform3f(u32 p_uniformLocation, f32 values[3]) const = 0;
		virtual void SetUniform4f(u32 p_uniformLocation, f32 values[4]) const = 0;
		virtual void SetUniformMatrix4f(u32 p_uniformLocation, const f32* values) const = 0;

		static std::shared_ptr<Shader> Create(const std::string& p_vertexShaderSource,
			                                  const std::string& p_fragmentShaderSource);

	};

}