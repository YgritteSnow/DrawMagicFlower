#pragma once
#include "MakeCircle.h"

JPoint rotate(const JPoint& origin, const JPoint& direction);

JPoint rotate(const JPoint& origin, double ang);

JPoint rotate_and_translate(const JPoint& origin, const JPoint& position, const JPoint& direction);

JPoint translate_and_rotate(const JPoint& origin, const JPoint& position, const JPoint& direction);

double cal_angle(const JPoint& cur, const JPoint& succ, const JPoint& next);

double cal_angle(const CurvePoint& cur, const CurvePoint& succ, const CurvePoint& next);

JPoint normalise(const JPoint& ori);

void DebugDrawPoint(const JPoint& point, const cv::Scalar& color);
void DebugDrawDirect(const JPoint& point, const JPoint& direct, const cv::Scalar& color);