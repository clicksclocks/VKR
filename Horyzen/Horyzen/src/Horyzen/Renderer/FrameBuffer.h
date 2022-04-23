#pragma once

#include "Horyzen/Core.h"
#include "ColorFormats.h"


namespace Horyzen {

	enum class FrameBufferAttachmentFormat
	{
		None = 0,

		// Color
		RGBA8,

		// Depth
		Depth16,
		Depth24,
		Depth32,
		Depth32F,

		// Depth + Stencil
		Depth24Stencil8,
		Depth32FStencil8,

		// Defaults
		Depth = Depth24Stencil8
	};

	enum class FrameBufferAttachmentFilterOption
	{
		Nearest,
		Linear,
		NearestMipmapNearest,
		LinearMipmapNearest,
		NearestMipmapLinear,
		LinearMipmapLinear
	};

	enum class FrameBufferAttachmentWrapOption
	{
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder,
		MirrorClampToEdge,
	};

	struct FrameBufferAttachmentSpecification
	{
		FrameBufferAttachmentSpecification() = default;
		inline FrameBufferAttachmentSpecification(FrameBufferAttachmentFormat p_format)
			: Format(p_format)
		{}

		inline FrameBufferAttachmentSpecification(
			FrameBufferAttachmentFormat       p_format,
			FrameBufferAttachmentWrapOption   p_wrapOption,
			FrameBufferAttachmentFilterOption p_filterOption)
			: Format(p_format),
			  WrapS(p_wrapOption),
			  WrapT(p_wrapOption),
			  MinFilter(p_filterOption),
			  MagFilter(p_filterOption)
		{}

		FrameBufferAttachmentFormat Format = FrameBufferAttachmentFormat::None;
		FrameBufferAttachmentWrapOption WrapS = FrameBufferAttachmentWrapOption::ClampToEdge;
		FrameBufferAttachmentWrapOption WrapT = FrameBufferAttachmentWrapOption::ClampToEdge;
		FrameBufferAttachmentFilterOption MinFilter = FrameBufferAttachmentFilterOption::Linear;
		FrameBufferAttachmentFilterOption MagFilter = FrameBufferAttachmentFilterOption::Linear;
	};

	struct FrameBufferSpecification
	{
		std::vector<FrameBufferAttachmentSpecification> AttachmentSpecs;
		u32 Width{ 0 };
		u32 Height{ 0 };
		u16 Samples{ 1 };
	};

	class HORYZEN_OBJECT FrameBuffer
	{
	public:
		
		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual u32 GetColorAttachmentID(u64 p_attachmentIndex) = 0;
		virtual void BindColorAttachmentToUnit(u64 p_attachmentIndex, u32 p_bindingIndex) = 0;
		virtual void ClearColorAttachment(u64 p_attachmentIndex, f32 p_r, f32 p_g, f32 p_b, f32 p_a) = 0;
		virtual Color::RGBAu ReadPixelRGBAu(u64 p_attachmentIndex, u32 p_x, u32 p_y) = 0;
		virtual Color::RGBAf ReadPixelRGBAf(u64 p_attachmentIndex, u32 p_x, u32 p_y) = 0;
		virtual Color::RGBu ReadPixelRGBu(u64 p_attachmentIndex, u32 p_x, u32 p_y) = 0;
		virtual Color::RGBf ReadPixelRGBf(u64 p_attachmentIndex, u32 p_x, u32 p_y) = 0;
		
		virtual void Resize(u32 p_width, u32 p_height) = 0;

		static std::shared_ptr<FrameBuffer> Create(const FrameBufferSpecification& p_properties);

		static void Blit(std::shared_ptr<FrameBuffer> p_destinationFBO,
			             u64                          p_destinationAttachmentIndex,
			             std::shared_ptr<FrameBuffer> p_sourceFBO,
			             u64                          p_sourceAttachmentIndex);

	private:

		virtual void Blit(u64                          p_sourceAttachmentIndex,
			              std::shared_ptr<FrameBuffer> p_destinationFBO,
			              u64                          p_destinationAttachmentIndex) = 0;

	};

}