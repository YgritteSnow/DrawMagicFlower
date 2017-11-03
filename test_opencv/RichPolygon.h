#pragma once
#include <vector>

#include "MakeCircle.h"

struct PolyRawSub{
	PolygonRaw* poly;
	JPoint last_position;
};

class RichPolygon
{
public:
	RichPolygon(PolygonRaw* m) : m_mainPolygon(m) {}
	~RichPolygon() { m_mainPolygon = nullptr; }
public:
	void AddPolygonRaw(PolygonRaw* sub_poly);
public:
	PolygonRaw* m_mainPolygon;
	std::vector<PolyRawSub> m_subPolygon;
};