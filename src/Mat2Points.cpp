#include "Mat2Points.h"



Mat2Points::Mat2Points()
{
	_stride = 1;
}

Mat2Points::~Mat2Points()
{

}

/*
Process an image and return points as vector.
@param mat - a 2d matrix with surface coordinates of the object of interest. 
@param points - a pointer to a vector to store the points as (x, y, z). 
@param normals - a pointer to a vector to store the normals as (x, y, z). 
*/
void Mat2Points::process(cv::Mat& mat, std::vector<Eigen::Vector3f>* points, std::vector<Eigen::Vector3f>* normals)
{
	int cols = mat.cols;
	int rows = mat.rows;

	points->clear();
	normals->clear();
	cv::Mat normals_mat;

	// compute normals
	_nc.process(mat, normals_mat);


	for (int i = 0; i < rows; i += _stride) {
		for (int j = 0; j < cols; j += _stride) {
			if (i < rows && j < cols) {
				cv::Vec3f p = mat.at<cv::Vec3f>(i, j);
				float sum = p[0] + p[1] + p[2];
				if (sum != 0.0) {
					Eigen::Vector3f p2 = Eigen::Vector3f(p[2], p[1], p[0]);
					points->push_back(p2);

					cv::Vec3f n = normals_mat.at<cv::Vec3f>(i, j);
					normals->push_back( Eigen::Vector3f(n[2], n[1], n[0]));
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