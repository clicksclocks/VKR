#include "pchheader.h"
#include "OpenGLFrameBuffer.h"
#include "../../Horyzen/Logger.h"

#include <glad/glad.h>

namespace Horyzen {

	namespace Utils {

		static bool IsDepthAttachment(FrameBufferAttachmentSpecification p_attachmentSpec)
		{
			switch (p_attachmentSpec.Format) {
				case FrameBufferAttachmentFormat::Depth16:          return true;
				case FrameBufferAttachmentFormat::Depth24:          return true;
				case FrameBufferAttachmentFormat::Depth32:          return true;
				case FrameBufferAttachmentFormat::Depth32F:         return true;
				case FrameBufferAttachmentFormat::Depth24Stencil8:  return true;
				case FrameBufferAttachmentFormat::Depth32FStencil8: return true;
			}
			return false;
		}

		static GLenum HoryzenAttachmentFormatToOpenGLAttachmentFormat(FrameBufferAttachmentFormat p_format)
		{
			switch (p_format) {
				case FrameBufferAttachmentFormat::RGBA8:            return GL_RGBA8;
				case FrameBufferAttachmentFormat::Depth16:          return GL_DEPTH_COMPONENT16;
				case FrameBufferAttachmentFormat::Depth24:          return GL_DEPTH_COMPONENT24;
				case FrameBufferAttachmentFormat::Depth32:          return GL_DEPTH_COMPONENT32;
				case FrameBufferAttachmentFormat::Depth32F:         return GL_DEPTH_COMPONENT32F;
				case FrameBufferAttachmentFormat::Depth24Stencil8:  return GL_DEPTH24_STENCIL8;
				case FrameBufferAttachmentFormat::Depth32FStencil8: return GL_DEPTH32F_STENCIL8;
			}
			return false;
		}

		static GLenum HoryzenFilterOptionsToOpenGLFilterOptions(FrameBufferAttachmentFilterOption p_filter)
		{
			switch (p_filter) {
				case FrameBufferAttachmentFilterOption::Linear:               return GL_NEAREST;
				case FrameBufferAttachmentFilterOption::LinearMipmapLinear:   return GL_LINEAR_MIPMAP_LINEAR;
				case FrameBufferAttachmentFilterOption::LinearMipmapNearest:  return GL_LINEAR_MIPMAP_NEAREST;
				case FrameBufferAttachmentFilterOption::Nearest:              return GL_LINEAR;
				case FrameBufferAttachmentFilterOption::NearestMipmapLinear:  return GL_NEAREST_MIPMAP_LINEAR;
				case FrameBufferAttachmentFilterOption::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
			}
			return false;
		}

		static GLenum HoryzenWrapOptionToOpenGLWrapOption(FrameBufferAttachmentWrapOption p_wrap)
		{
			switch (p_wrap) {
				case FrameBufferAttachmentWrapOption::Repeat:            return GL_REPEAT;
				case FrameBufferAttachmentWrapOption::MirroredRepeat:    return GL_MIRRORED_REPEAT;
				case FrameBufferAttachmentWrapOption::MirrorClampToEdge: return GL_MIRROR_CLAMP_TO_EDGE;
				case FrameBufferAttachmentWrapOption::ClampToEdge:       return GL_CLAMP_TO_EDGE;
				case FrameBufferAttachmentWrapOption::ClampToBorder:     return GL_CLAMP_TO_BORDER;
			}
			return false;
		}

	}


	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& p_properties)
		: m_Specification(p_properties)
	{
		for (auto& attachmentSpec : p_properties.AttachmentSpecs) {
			if (!Utils::IsDepthAttachment(attachmentSpec)) {
				m_ColorAttachmentSpecs.push_back(attachmentSpec);
			} else {
				m_DepthAttachmentSpec = attachmentSpec;
			}
		}

		m_ColorAttachmentIDs.resize(m_ColorAttachmentSpecs.size());
		
		Invalidate();
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		if (m_ID) {
			glDeleteFramebuffers(1, &m_ID);
			glDeleteTextures(m_ColorAttachmentIDs.size(), m_ColorAttachmentIDs.data());
			glDeleteTextures(1, &m_DepthAttachmentID);
		}

		glCreateFramebuffers(1, &m_ID);

		if (m_Specification.Samples == 1) {
			glCreateTextures(GL_TEXTURE_2D, m_ColorAttachmentIDs.size(), m_ColorAttachmentIDs.data());
		} else {
			glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, m_ColorAttachmentIDs.size(), m_ColorAttachmentIDs.data());
		}

		for (size_t i = 0; i < m_ColorAttachmentSpecs.size(); ++i) {
			if (m_Specification.Samples == 1) {
				glTextureStorage2D(
					m_ColorAttachmentIDs[i],
					1,
					Utils::HoryzenAttachmentFormatToOpenGLAttachmentFormat(m_ColorAttachmentSpecs[i].Format),
					m_Specification.Width,
					m_Specification.Height);
			} else {
				glTextureStorage2DMultisample(
					m_ColorAttachmentIDs[i],
					m_Specification.Samples,
					Utils::HoryzenAttachmentFormatToOpenGLAttachmentFormat(m_ColorAttachmentSpecs[i].Format),
					m_Specification.Width,
					m_Specification.Height,
					GL_FALSE
				);
			}

			if (m_Specification.Samples == 1) {

				glTextureParameteri(
					m_ColorAttachmentIDs[i],
					GL_TEXTURE_WRAP_S,
					Utils::HoryzenWrapOptionToOpenGLWrapOption(m_ColorAttachmentSpecs[i].WrapS));
				glTextureParameteri(
					m_ColorAttachmentIDs[i],
					GL_TEXTURE_WRAP_T,
					Utils::HoryzenWrapOptionToOpenGLWrapOption(m_ColorAttachmentSpecs[i].WrapT));

				glTextureParameteri(
					m_ColorAttachmentIDs[i],
					GL_TEXTURE_MAG_FILTER,
					Utils::HoryzenFilterOptionsToOpenGLFilterOptions(m_ColorAttachmentSpecs[i].MagFilter));
				glTextureParameteri(
					m_ColorAttachmentIDs[i],
					GL_TEXTURE_MIN_FILTER,
					Utils::HoryzenFilterOptionsToOpenGLFilterOptions(m_ColorAttachmentSpecs[i].MinFilter));
			}

			glNamedFramebufferTexture(m_ID, GL_COLOR_ATTACHMENT0 + i, m_ColorAttachmentIDs[i], 0);
		}

		if (m_DepthAttachmentSpec.Format != FrameBufferAttachmentFormat::None) {
			if (m_Specification.Samples == 1) {
				glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachmentID);
				glTextureStorage2D(
					m_DepthAttachmentID,
					1,
					Utils::HoryzenAttachmentFormatToOpenGLAttachmentFormat(m_DepthAttachmentSpec.Format),
					m_Specification.Width,
					m_Specification.Height);
			}
			else {
				glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_DepthAttachmentID);
				glTextureStorage2DMultisample(
					m_DepthAttachmentID,
					m_Specification.Samples,
					Utils::HoryzenAttachmentFormatToOpenGLAttachmentFormat(m_DepthAttachmentSpec.Format),
					m_Specification.Width,
					m_Specification.Height,
					GL_FALSE);
			}
			glNamedFramebufferTexture(m_ID, GL_DEPTH_STENCIL_ATTACHMENT, m_DepthAttachmentID, 0);
		}
		
		std::vector<GLenum> attachmentsToDraw(m_ColorAttachmentIDs.size());
		for (size_t i = 0; i < attachmentsToDraw.size(); ++i) {
			attachmentsToDraw[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		glNamedFramebufferDrawBuffers(m_ID, attachmentsToDraw.size(), attachmentsToDraw.data());

		auto status = glCheckNamedFramebufferStatus(m_ID, GL_FRAMEBUFFER);
		switch (status) {
			case GL_FRAMEBUFFER_COMPLETE: break;

			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			{
				HORYZEN_LOG_CRITICAL("FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
			} break;

			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			{
				HORYZEN_LOG_CRITICAL("FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
			} break;

			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			{
				HORYZEN_LOG_CRITICAL("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
			} break;

			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			{
				HORYZEN_LOG_CRITICAL("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
			} break;

			case GL_FRAMEBUFFER_UNSUPPORTED:
			{
				HORYZEN_LOG_CRITICAL("FRAMEBUFFER_UNSUPPORTED");
			} break;

			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			{
				HORYZEN_LOG_CRITICAL("FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
			} break;

			default:
			{
				HORYZEN_LOG_CRITICAL("Unknown incompleteness error!");
			}
		}
		HORYZEN_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Incomplete buffer!");
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(m_ColorAttachmentIDs.size(), m_ColorAttachmentIDs.data());
		glDeleteTextures(1, &m_DepthAttachmentID);
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	}

	void OpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	u32 OpenGLFrameBuffer::GetColorAttachmentID(u64 p_attachmentIndex)
	{
		return m_ColorAttachmentIDs[p_attachmentIndex];
	}

	void OpenGLFrameBuffer::BindColorAttachmentToUnit(u64 p_attachmentIndex, u32 p_bindingIndex)
	{
		u32 ID = m_ColorAttachmentIDs[p_attachmentIndex];
		glBindTextureUnit(p_bindingIndex, ID);
	}

	void OpenGLFrameBuffer::ClearColorAttachment(u64 p_attachmentIndex, f32 p_r, f32 p_g, f32 p_b, f32 p_a)
	{
		f32 color[4] = { p_r, p_g, p_b, p_a };
		glClearTexImage(m_ColorAttachmentIDs[p_attachmentIndex], 0, GL_RGBA, GL_FLOAT, color);
	}

	Color::RGBAu OpenGLFrameBuffer::ReadPixelRGBAu(u64 p_attachmentIndex, u32 p_x, u32 p_y)
	{
		Color::RGBAu o_color;
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		glNamedFramebufferReadBuffer(m_ID, GL_COLOR_ATTACHMENT0 + p_attachmentIndex);
		glReadPixels(p_x, p_y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &o_color);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return o_color;
	}

	Color::RGBAf OpenGLFrameBuffer::ReadPixelRGBAf(u64 p_attachmentIndex, u32 p_x, u32 p_y)
	{
		Color::RGBAf o_color;
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		glNamedFramebufferReadBuffer(m_ID, GL_COLOR_ATTACHMENT0 + p_attachmentIndex);
		glReadPixels(p_x, p_y, 1, 1, GL_RGBA, GL_FLOAT, &o_color);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return o_color;
	}

	Color::RGBu OpenGLFrameBuffer::ReadPixelRGBu(u64 p_attachmentIndex, u32 p_x, u32 p_y)
	{
		Color::RGBu o_color;
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		glNamedFramebufferReadBuffer(m_ID, GL_COLOR_ATTACHMENT0 + p_attachmentIndex);
		glReadPixels(p_x, p_y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &o_color);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return o_color;
	}

	Color::RGBf OpenGLFrameBuffer::ReadPixelRGBf(u64 p_attachmentIndex, u32 p_x, u32 p_y)
	{
		Color::RGBf o_color;
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
		glNamedFramebufferReadBuffer(m_ID, GL_COLOR_ATTACHMENT0 + p_attachmentIndex);
		glReadPixels(p_x, p_y, 1, 1, GL_RGB, GL_FLOAT, &o_color);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return o_color;
	}

	void OpenGLFrameBuffer::Blit(u64                          p_sourceAttachmentIndex,
		                         std::shared_ptr<FrameBuffer> p_destinationFBO,
		                         u64                          p_destinationAttachmentIndex)
	{
		auto destinatiobFBO = std::dynamic_pointer_cast<OpenGLFrameBuffer>(p_destinationFBO);

		glNamedFramebufferReadBuffer(m_ID, GL_COLOR_ATTACHMENT0 + p_sourceAttachmentIndex);

		std::vector<GLenum> drawAttachment(p_destinationAttachmentIndex + 1, GL_NONE);
		drawAttachment[p_destinationAttachmentIndex] = GL_COLOR_ATTACHMENT0 + p_destinationAttachmentIndex;
		glNamedFramebufferDrawBuffers(destinatiobFBO->GetID(), drawAttachment.size(), drawAttachment.data());

		//f32 color[4] = { 0.0, 0.0, 0.0, 1.0 };
		//glClearNamedFramebufferfv(destinatiobFBO->GetID(), GL_COLOR, p_destinationAttachmentIndex, color);

		
		glBlitNamedFramebuffer(m_ID,                      // Read buffer
			                   destinatiobFBO->GetID(),   // Draw buffer
			                   0,                         // Source X0
			                   0,                         // Source Y0
			                   m_Specification.Width,     // Source X1
			                   m_Specification.Height,    // Source Y1
			                   0,                         // Destination X0 = Source X0
			                   0,                         // Destination Y0 = Source Y0
			                   m_Specification.Width,     // Destination X1 = Source X1
			                   m_Specification.Height,    // Destination Y1 = Source Y1
			                   GL_COLOR_BUFFER_BIT,       // Buffers to copy
			                   GL_LINEAR);

	}

	void OpenGLFrameBuffer::Resize(u32 p_width, u32 p_height)
	{
		m_Specification.Width = p_width;
		m_Specification.Height = p_height;

		Invalidate();
	}

}
