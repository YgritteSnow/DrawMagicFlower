#include <opencv2/opencv.hpp>
#include "DrawMakeCircle.h"
#include "Utility.h"

extern cv::Mat g_dynamicImg;
extern cv::Mat g_staticImg;

/************************************************************************/
/*                                                                      */
/************************************************************************/

void DrawPolygonRaw::Draw(const PolygonRaw* polygon)
{
	for (size_type i = 0; i < polygon->m_uCount; ++i)
	{
		line(g_dynamicImg, polygon->CalPositionWithTrans(i), polygon->CalPositionWithTrans(i + 1)
			, cv::Scalar(1,1,1), 1);
	}
}

void DrawPolygonRaw::Draw(RichPolygon* rich)
{
	Draw(rich->m_mainPolygon);
	for (auto iter = rich->m_subPolygon.begin(); iter != rich->m_subPolygon.end(); ++iter)
	{
		for (size_type i = 0; i < iter->poly->m_uCount; ++i)
		{
			JPoint bgn = rotate_and_translate(iter->poly->CalPositionWithTrans(i), rich->m_mainPolygon->m_position, rich->m_mainPolygon->m_direction);
			JPoint end = rotate_and_translate(iter->poly->CalPositionWithTrans(i + 1), rich->m_mainPolygon->m_position, rich->m_mainPolygon->m_direction);
			line(g_dynamicImg, bgn, end, cv::Scalar(1, 1, 1), 1);
		}

		JPoint new_position = rotate_and_translate(iter->poly->CalPositionWithTrans(0), rich->m_mainPolygon->m_position, rich->m_mainPolygon->m_direction);
		if (iter->last_position.y > 0)
		{
			line(g_staticImg, iter->last_position, new_position, cv::Scalar(0, 0, 255), 2);
		}
		iter->last_position = new_position;
	}
}