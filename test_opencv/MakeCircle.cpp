#include <math.h>

#include "MakeCircle.h"
#include "Utility.h"

#include <opencv.hpp> // debugÓÃ£¬ÉÔºóÉ¾µô

/************************************************************************/
/*                                                                      */
/************************************************************************/
const double pi_m2 = std::acos(-1) * 2;

PolygonRaw::PolygonRaw()
: m_pPoints(nullptr)
, m_uCount(0)
, m_position(0, 0)
, m_direction(1, 0)
{}

PolygonRaw::PolygonRaw(const PolygonRaw& copy)
{
	m_uCount = copy.m_uCount;
	m_position = copy.m_position;
	m_direction = copy.m_direction;
	m_pPoints = new CurvePoint[m_uCount];
	memcpy(m_pPoints, copy.m_pPoints, sizeof(CurvePoint)* m_uCount);
}

PolygonRaw::~PolygonRaw()
{
	if (m_pPoints)
	{
		delete m_pPoints;
		m_pPoints = nullptr;
	}
	m_uCount = 0;
}

JPoint PolygonRaw::CalPositionWithTrans(size_type index) const
{
	CurvePoint& cuv_bgn = *(m_pPoints + index % m_uCount);
	JPoint& bgn = JPoint(cuv_bgn.posx, cuv_bgn.posy);
	return rotate_and_translate(bgn, m_position, m_direction);
}

JPoint PolygonRaw::CalPositionWithTrans(const PolygonPoint& poly_pos) const
{
	JPoint bgn = JPoint(poly_pos.head_point->posx, poly_pos.head_point->posy);
	JPoint end = JPoint(poly_pos.toe_point->posx, poly_pos.toe_point->posy);
	JPoint cur = bgn * (1-poly_pos.interp) + end * poly_pos.interp;
	return rotate_and_translate(cur, m_position, m_direction);
}

JPoint PolygonRaw::CalPositionNoTrans(const PolygonPoint& poly_pos) const
{
	JPoint bgn = JPoint(poly_pos.head_point->posx, poly_pos.head_point->posy);
	JPoint end = JPoint(poly_pos.toe_point->posx, poly_pos.toe_point->posy);
	return bgn * (1 - poly_pos.interp) + end * poly_pos.interp;
}

JPoint PolygonRaw::CalPositionNoTrans(size_type index) const
{
	CurvePoint& cuv_bgn = *(m_pPoints + index % m_uCount);
	return JPoint(cuv_bgn.posx, cuv_bgn.posy);
}

JPoint PolygonRaw::CalDirectionNoTrans(const PolygonPoint& poly_pos) const
{
	size_type head_idx = poly_pos.head_idx;
	JPoint head_succ = CalPositionNoTrans(head_idx - 1);
	JPoint head = CalPositionNoTrans(head_idx);
	JPoint toe = CalPositionNoTrans(head_idx + 1);
	JPoint toe_next = CalPositionNoTrans(head_idx + 2);

	JPoint head_dir = normalise(head_succ - head);
	JPoint toe_dir = normalise(toe_next - toe);
	JPoint cur_dir = normalise(toe - head);

	return normalise((head_dir * (1 - poly_pos.interp) + toe_dir * poly_pos.interp + cur_dir * (1 - poly_pos.interp)) / 2);
}

JPoint PolygonRaw::CalDirectionWithTrans(const PolygonPoint& poly_pos) const
{
	size_type head_idx = poly_pos.head_idx;
	JPoint head_succ = CalPositionWithTrans(head_idx - 1);
	JPoint head = CalPositionWithTrans(head_idx);
	JPoint toe = CalPositionWithTrans(head_idx + 1);
	JPoint toe_next = CalPositionWithTrans(head_idx + 2);

	JPoint head_dir = normalise(head_succ - head);
	JPoint toe_dir = normalise(toe_next - toe);
	JPoint cur_dir = normalise(toe - head);

	return normalise((head_dir * (1-poly_pos.interp) + toe_dir * poly_pos.interp + cur_dir * (1-poly_pos.interp)) / 2);
}

void PolygonRaw::TransformByIntersectInfo(IntersectInfo intersect_info)
{
	JPoint new_point = intersect_info.outer_poly->CalPositionWithTrans(intersect_info.outer_point);
	JPoint new_direct = intersect_info.outer_poly->CalDirectionWithTrans(intersect_info.outer_point);
	double new_ang = std::atan2(new_direct.y, new_direct.x);
	DebugDrawPoint(new_point, cv::Scalar(255, 0, 0));
	DebugDrawDirect(new_point, new_direct, cv::Scalar(255, 0, 0));

	JPoint old_point = CalPositionNoTrans(intersect_info.inner_point);
	JPoint old_direct = CalDirectionNoTrans(intersect_info.inner_point);
	DebugDrawPoint(rotate_and_translate(old_point, m_position, m_direction), cv::Scalar(0, 255, 0));
	DebugDrawDirect(rotate_and_translate(old_point, m_position, m_direction), rotate(old_direct, m_direction), cv::Scalar(0, 255, 0));
	double old_ang = std::atan2(old_direct.y, old_direct.x);

	double ang_delta = new_ang - old_ang;

	m_direction = JPoint(std::cos(new_ang - old_ang), std::sin(new_ang - old_ang));

	JPoint tmp_point = rotate(old_point, m_direction);
	m_position = new_point - tmp_point;

}

PolygonRaw PolygonRaw::RandomOuter(JPoint position, size_type precis, double radius)
{
	PolygonRaw result = PolygonRaw();
	result.m_position = position;
	result.m_uCount = precis;
	result.m_pPoints = new CurvePoint[result.m_uCount];
	double ang = pi_m2 / result.m_uCount;
	for (size_type i = 0; i < result.m_uCount; ++i)
	{
		CurvePoint& p = result.m_pPoints[i];
		p.posx = std::cos(ang * i) * radius;
		p.posy = std::sin(ang * i) * radius;
	}
	return result;
}

PolygonRaw PolygonRaw::RandomInner(JPoint position, size_type precis, double radius)
{
	PolygonRaw result = PolygonRaw();
	result.m_position = position;
	result.m_uCount = precis;
	result.m_pPoints = new CurvePoint[result.m_uCount];
	double ang = pi_m2 / result.m_uCount;
	for (size_type i = 0; i < result.m_uCount; ++i)
	{
		CurvePoint& p = result.m_pPoints[i];
		p.posx = std::cos(ang * i) * radius * (std::cos(ang * i) * 0.2 + 0.8);
		p.posy = std::sin(ang * i) * radius * (std::sin(ang * i) * 0.2 + 0.8);
	}
	for (size_type i = 0; i < result.m_uCount; ++i)
	{
		CurvePoint& p = result.m_pPoints[i];
		CurvePoint& p_succ = result.m_pPoints[(i + result.m_uCount - 1)%result.m_uCount];
		CurvePoint& p_next = result.m_pPoints[(i + 1) % result.m_uCount];
		p.curvature = cal_angle(p, p_succ, p_next);
	}
	return result;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

PolyIntersect::PolyIntersect(PolygonRaw* o, PolygonRaw* i)
: outer(o)
, inner(i)
{}

PolyIntersect::~PolyIntersect()
{
	outer = nullptr;
	inner = nullptr;
}

IntersectInfo PolyIntersect::NextIntersect()
{
	IntersectInfo result = IntersectInfo();
	result.inner_poly = inner;
	result.outer_poly = outer;
	static double inner_idx_d = 0;
	static double outer_idx_d = 0;
	static size_type inner_idx = 0;//(size_type)inner_idx_d;
	static size_type outer_idx = 0;//(size_type)outer_idx_d;
	result.inner_point = PolygonPoint(inner->m_pPoints + inner_idx, inner->m_pPoints + 1 + inner_idx, 0, inner_idx);
	result.outer_point = PolygonPoint(outer->m_pPoints + outer_idx, outer->m_pPoints + 1 + outer_idx, 0, outer_idx);

	inner_idx = (inner_idx + 3) % inner->m_uCount;
	outer_idx = (outer_idx + 2) % outer->m_uCount;

	inner_idx_d += 3;
	outer_idx_d += 1;
	if (inner_idx_d >= inner->m_uCount) inner_idx_d -= inner->m_uCount;
	if (outer_idx_d >= outer->m_uCount) outer_idx_d -= outer->m_uCount;
	return result;
}