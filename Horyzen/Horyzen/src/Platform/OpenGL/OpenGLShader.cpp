#include "pchheader.h"
#include "OpenGLShader.h"

#include "Horyzen/Logger.h"

#include <glad/glad.h>

namespace Horyzen {\

	OpenGLShader::OpenGLShader(const std::string& p_vertexShaderSource, const std::string& p_fragmentShaderSource)
	{
        // Создаём две формы для выпечки с рисунком
        u32 vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        u32 fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);


        std::vector<char> vertex_code;

        const GLchar* vertex_source = (const GLchar*)p_vertexShaderSource.c_str();
        // Наполняем форму для выпечки vertex_shader_id тестом vertex_source
        glShaderSource(vertexShaderID, 1, &vertex_source, 0);

        const GLchar* fragment_source = (const GLchar*)p_fragmentShaderSource.c_str();
        // Наполняем форму для выпечки fragment_shader_id тестом fragment_source
        glShaderSource(fragmentShaderID, 1, &fragment_source, 0);


        // Испекаем vertex_shader_id, в котором находится тесто vertex_source
        glCompileShader(vertexShaderID);
        GLint isCompiled = 0;
        glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &isCompiled);

        // Дегустируем испечённое блюдо. Если оно получилось испорченное, плюём повару в лицо
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShaderID, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(vertexShaderID);

            HORYZEN_LOG_ERROR("Shader compilation error:\n{0}", infoLog.data());

            // Use the infoLog as you see fit.

            // In this simple program, we'll just leave
        }

        // Испекаем fragment_shader_id, в котором находится тесто fragment_source
        glCompileShader(fragmentShaderID);
        isCompiled = 0;
        glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &isCompiled);

        // Дегустируем испечённое блюдо. Если оно получилось испорченное, плюём повару в лицо
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShaderID, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(fragmentShaderID);

            HORYZEN_LOG_ERROR("Shader compilation error:\n{0}", infoLog.data());

            // Use the infoLog as you see fit.

            // In this simple program, we'll just leave
        }


        // Создаём проектор
        u32 shaderProgramID = glCreateProgram();

        // Засовываем в проектор два испечённых блюда
        glAttachShader(shaderProgramID, vertexShaderID);
        glAttachShader(shaderProgramID, fragmentShaderID);

        // Склеиваем испечённые блюда чем-нибудь
        glLinkProgram(shaderProgramID);

        // Если кто-то склеил их соплями и проектор развалился, выяясняем, какого хрена
        GLint isLinked = 0;
        glGetProgramiv(shaderProgramID, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(shaderProgramID, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(shaderProgramID);
            // Don't leak shaders either.

            HORYZEN_LOG_ERROR("Shader compilation error:\n{0}", infoLog.data());
            // Use the infoLog as you see fit.

            // In this simple program, we'll just leave
        }

        // После успешного склеивания можно вытащить сами формы для выпечки.
        // Приготовленные блюда уже находятся в проекторе и склеены.
        glDetachShader(shaderProgramID, vertexShaderID);
        glDetachShader(shaderProgramID, fragmentShaderID);

        // Поскольку формы для выпечки нам больше не нужны, их можно удалить
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        m_ID = shaderProgramID;
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_ID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

    void OpenGLShader::SetUniform1f(u32 p_uniformLocation, f32 value) const
    {
        glUniform1f(static_cast<i32>(p_uniformLocation), value);
    }

    void OpenGLShader::SetUniform2f(u32 p_uniformLocation, f32 values[2]) const
    {
        glUniform2f(static_cast<i32>(p_uniformLocation), values[0], values[1]);
    }

    void OpenGLShader::SetUniform3f(u32 p_uniformLocation, f32 values[3]) const
    {
        glUniform3f(static_cast<i32>(p_uniformLocation), values[0], values[1], values[2]);
    }

    void OpenGLShader::SetUniform4f(u32 p_uniformLocation, f32 values[4]) const
    {
        glUniform4f(static_cast<i32>(p_uniformLocation), values[0], values[1], values[2], values[3]);
    }

    void OpenGLShader::SetUniformMatrix4f(u32 p_uniformLocation, const f32* values) const
    {
         glUniformMatrix4fv(p_uniformLocation, 1, false, values);
    }

}
