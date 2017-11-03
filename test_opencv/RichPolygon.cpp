#include "RichPolygon.h"

void RichPolygon::AddPolygonRaw(PolygonRaw* sub_poly)
{
	m_subPolygon.push_back(PolyRawSub{sub_poly, JPoint(-1, -1)});
}