#pragma once

#include "Horyzen/Math/Vec4.h"
#include "Horyzen/Math/Vec3.h"

#include "Horyzen/Renderer/ColorFormats.h"

#include "Horyzen/Core.h"

namespace Horyzen::Geo {

	class GObject;
	class GPoint
	{	
		friend class GPointCollection;
		friend class GPointPickingSystem;

		using Reference = std::pair<GObject*, Vec4D*>;

		std::vector<Reference> m_References;
		GPointCollection* m_CollectionReference;

		Color::RGBu m_Color;
		u64 m_ID;
		Color::RGBu m_ColorID;

	public:

		GPoint(GPointCollection* p_collection);

		static Color::RGBu IDToColorID(u64 p_ID);
		static u64 ColorIDToID(Color::RGBu p_color);

		void AddReference(Reference p_reference);

	};

}



