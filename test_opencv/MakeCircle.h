#pragma once
#include <vector>
#include <opencv2/opencv.hpp>

/************************************************************************/
/*                                                                      */
/************************************************************************/
const double MAX_CURVATURE = 99999;
typedef unsigned int size_type;
typedef cv::Point2d JPoint;

struct CurvePoint{
	CurvePoint() :posx(0), posy(0), curvature(0){};
	double posx;
	double posy;
	double curvature; // 0 for simple point; MAX for line; other for curve.
	JPoint GetRaw()
	{
		return JPoint(posx, posy);
	}
};

struct Vector2{
	Vector2() :posx(0), posy(0){}
	float posx;
	float posy;
};

struct PolygonPoint{
	PolygonPoint()
		: head_point(nullptr)
		, toe_point(nullptr)
		, interp(0)
		, head_idx(0)
		{};
	PolygonPoint(CurvePoint* h, CurvePoint* t, double i, size_type d)
		: head_point(h)
		, toe_point(t)
		, interp(i)
		, head_idx(d)
		{};
	CurvePoint* head_point; // 交点的上一个点
	CurvePoint* toe_point; // 交点的下一个点
	double interp; // 两点之间的插值，head处为0，toe处为1
	size_type head_idx; // head的索引
};

/************************************************************************/
/*                                                                      */
/************************************************************************/

class PolygonRaw;
struct IntersectInfo{
	IntersectInfo()
	: outer_point()
	, inner_point()
	, outer_poly(nullptr)
	, inner_poly(nullptr)
	{};

	PolygonPoint outer_point;
	PolygonPoint inner_point;
	PolygonRaw* outer_poly;
	PolygonRaw* inner_poly;
};

class PolygonRaw
{
public:
	PolygonRaw();
	PolygonRaw(const PolygonRaw& copy);
	~PolygonRaw();

public:
	size_type m_uCount;
	CurvePoint* m_pPoints;
public:
	JPoint CalPositionNoTrans(const PolygonPoint& poly_pos) const;
	JPoint CalPositionNoTrans(size_type index) const;
	JPoint CalPositionWithTrans(const PolygonPoint& poly_pos) const;
	JPoint CalPositionWithTrans(size_type index) const;
	JPoint CalDirectionWithTrans(const PolygonPoint& poly_pos) const;
	JPoint CalDirectionNoTrans(const PolygonPoint& poly_pos) const;
	// 根据碰撞信息，将本物体作为inner进行变换
	void TransformByIntersectInfo(IntersectInfo intersect_info);
public:
	JPoint m_position;
	JPoint m_direction;
public:
	static PolygonRaw RandomOuter(JPoint position, size_type precis, double radius);
	static PolygonRaw RandomInner(JPoint position, size_type precis, double radius);
};

/************************************************************************/
/*                                                                      */
/************************************************************************/

class PolyIntersect
{
public:
	PolyIntersect(PolygonRaw* o, PolygonRaw* i);
	~PolyIntersect();
public:
	IntersectInfo NextIntersect();
private:
	PolygonRaw* outer;
	PolygonRaw* inner;
};