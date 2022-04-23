#pragma once

#include "Horyzen/Renderer/FrameBuffer.h"

namespace Horyzen {

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:

		OpenGLFrameBuffer(const FrameBufferSpecification& p_properties);
		virtual ~OpenGLFrameBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual u32 GetColorAttachmentID(u64 p_attachmentIndex) override;
		virtual void BindColorAttachmentToUnit(u64 p_attachmentIndex, u32 p_bindingIndex) override;
		virtual void ClearColorAttachment(u64 p_attachmentIndex, f32 p_r, f32 p_g, f32 p_b, f32 p_a) override;

		virtual Color::RGBAu ReadPixelRGBAu(u64 p_attachmentIndex, u32 p_x, u32 p_y) override;
		virtual Color::RGBAf ReadPixelRGBAf(u64 p_attachmentIndex, u32 p_x, u32 p_y) override;
		virtual Color::RGBu ReadPixelRGBu(u64 p_attachmentIndex, u32 p_x, u32 p_y) override;
		virtual Color::RGBf ReadPixelRGBf(u64 p_attachmentIndex, u32 p_x, u32 p_y) override;

		virtual void Blit(u64                          p_sourceAttachmentIndex,
			              std::shared_ptr<FrameBuffer> p_destinationFBO,
			              u64                          p_destinationAttachmentIndex) override;

		virtual void Resize(u32 p_width, u32 p_height) override;
		
		void Invalidate();

		inline const u32 GetID() const { return m_ID; };

	private:

		FrameBufferSpecification m_Specification;
		u32 m_ID{ 0 };

		std::vector<u32> m_ColorAttachmentIDs;
		u32 m_DepthAttachmentID{ 0 };

		std::vector<FrameBufferAttachmentSpecification> m_ColorAttachmentSpecs;
		FrameBufferAttachmentSpecification m_DepthAttachmentSpec;

	};

}