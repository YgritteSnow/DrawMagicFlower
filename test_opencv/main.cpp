#include <opencv2/opencv.hpp>
#include <vector>

#include "DrawMakeCircle.h"
#include "MakeCircle.h"

using namespace cv;

const std::string WINDOW_NAME = "hahahahahahahah~";
Mat g_staticImg(800, 800, CV_8UC4);
Mat g_dynamicImg(800, 800, CV_8UC4);

void createImg(Mat &mat)
{
	for (int i = 0; i < mat.rows; ++i)
	for (int j = 0; j < mat.cols; ++j)
	{
		Vec4b & rgba = mat.at<Vec4b>(i,j);
		rgba[0] = 0xff;//saturate_cast<uchar>(rand() % 0xff);
		rgba[1] = 0xff;//saturate_cast<uchar>(rand() % 0xff);
		rgba[2] = 0xff;//saturate_cast<uchar>(rand() % 0xff);
		rgba[3] = 0xff;
	}
}

int main()
{
	createImg(g_staticImg);
	createImg(g_dynamicImg);

	PolygonRaw outer_data = PolygonRaw::RandomOuter(JPoint(300, 300), 100, 200);
	PolygonRaw inner_data = PolygonRaw::RandomInner(JPoint(0, 0), 100, 100);
	PolygonRaw inner_data_sub = PolygonRaw::RandomInner(JPoint(50, 0), 10, 3);
	RichPolygon rich_poly = RichPolygon(&inner_data);
	rich_poly.AddPolygonRaw(&inner_data_sub);
	PolyIntersect intesect(&outer_data, &inner_data);

	while (true)
	{
		g_staticImg.copyTo(g_dynamicImg);

		DrawPolygonRaw::Draw(&outer_data);
		DrawPolygonRaw::Draw(&rich_poly);
		IntersectInfo intersect_point = intesect.NextIntersect();
		inner_data.TransformByIntersectInfo(intersect_point);

		imshow(WINDOW_NAME, g_dynamicImg);
		waitKey(100);
	}
}