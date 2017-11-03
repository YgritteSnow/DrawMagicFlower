#include <opencv2/opencv.hpp>
#include "Utility.h"

extern cv::Mat g_dynamicImg;

JPoint rotate(const JPoint& origin, const JPoint& direction)
{
	return JPoint(origin.x * direction.x - origin.y * direction.y
		, origin.y * direction.x + origin.x * direction.y);
}

JPoint rotate(const JPoint& origin, double ang)
{
	double x = std::cos(ang);
	double y = std::sin(ang);
	return JPoint(origin.x * x - origin.y * y
		, origin.y * x + origin.x * y);
}

JPoint translate(const JPoint& origin, const JPoint& trans)
{
	return origin + trans;
}

JPoint rotate_and_translate(const JPoint& origin, const JPoint& position, const JPoint& direction)
{
	double x = origin.x * direction.x - origin.y * direction.y;
	double y = origin.y * direction.x + origin.x * direction.y;
	return JPoint(x + position.x, y + position.y);
}

JPoint translate_and_rotate(const JPoint& origin, const JPoint& position, const JPoint& direction)
{
	double x = (origin.x + position.x) * direction.x - (origin.y + position.y) * direction.y;
	double y = (origin.y + position.y) * direction.x + (origin.x + position.x) * direction.y;
	return JPoint(x, y);
}

double cal_angle(const JPoint& cur, const JPoint& succ, const JPoint& next)
{
	JPoint next_dir = next - cur;
	double next_ang = std::atan2(next_dir.y, next_dir.x);
	JPoint succ_dir = succ - cur;
	double succ_ang = std::atan2(succ_dir.y, succ_dir.x);
	return next_ang - succ_ang;
}

double cal_angle(const CurvePoint& cur, const CurvePoint& succ, const CurvePoint& next)
{
	double next_ang = std::atan2(next.posy - cur.posy, next.posx - cur.posx);
	double succ_ang = std::atan2(succ.posy - cur.posy, succ.posx - cur.posx);
	return next_ang - succ_ang;
}

JPoint normalise(const JPoint& ori)
{
	return ori / std::sqrt(ori.dot(ori));
}

void DebugDrawPoint(const JPoint& point, const cv::Scalar& color)
{
	cv::ellipse(g_dynamicImg, point, cv::Size(2, 2), 0, 0, 360, color, 3);
}

void DebugDrawDirect(const JPoint& point, const JPoint& direct, const cv::Scalar& color)
{
	JPoint end = point + direct * 50;
	cv::line(g_dynamicImg, end, point, color, 2);
}