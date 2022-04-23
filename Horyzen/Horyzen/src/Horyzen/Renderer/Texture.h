#pragma once

#include "Horyzen/Core.h"

namespace Horyzen {

	class HORYZEN_OBJECT Texture
	{
	public:

		virtual ~Texture() = default;

		virtual u32 GetWidth() const = 0;
		virtual u32 GetHeight() const = 0;

		virtual void Bind(u32 p_location) const = 0;
	
	};

	class HORYZEN_OBJECT Texture2D : public Texture
	{
	public:

		virtual ~Texture2D() = default;

		static std::shared_ptr<Texture2D> Create(const std::string& filePath);

	};

}