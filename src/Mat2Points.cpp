#include "Mat2Points.h"



Mat2Points::Mat2Points()
{
	_stride = 1;
}

Mat2Points::~Mat2Points()
{

}


/*
Process an image and return points in 
*/
void Mat2Points::process(cv::Mat& mat, std::vector<Eigen::Vector3f>* points)
{
	int cols = mat.cols;
	int rows = mat.rows;

	points->clear();

	for (int i = 0; i < rows; i += _stride) {
		for (int j = 0; j < cols; j += _stride) {
			if (i < rows && j < cols) {
				cv::Vec3f p = mat.at<cv::Vec3f>(i, j);
				float sum = p[0] + p[1] + p[2];
				if (sum != 0.0) {
					Eigen::Vector3f p2 = Eigen::Vector3f(p[2], p[1], p[0]);
					points->push_back(p2);
				}
			}
		}
	}
}


/*
Set the stride value for the matrix.
@param stride - integer >= 1.
*/
void Mat2Points::setStride(int stride)
{
	if (stride > 0) {
		_stride = stride;
	}
}