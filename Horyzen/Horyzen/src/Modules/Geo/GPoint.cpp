#include "pchheader.h"
#include "GPoint.h"

namespace Horyzen::Geo {

	GPoint::GPoint(GPointCollection* p_collection)
		: m_CollectionReference(p_collection)
	{}

	void GPoint::AddReference(Reference p_reference)
	{
		m_References.push_back(p_reference);
	}

	Color::RGBu GPoint::IDToColorID(u64 p_ID)
	{
		Color::RGBu color;
		color.r = (p_ID & 0xff);
		color.g = ((p_ID & 0xff00) >> 8);
		color.b = ((p_ID & 0xff0000) >> 16);
		return color;
	}

	u64 GPoint::ColorIDToID(Color::RGBu p_color)
	{
		return p_color.r | (p_color.g << 8) | (p_color.b << 16);
	}

}