#include "Mat2Normals.h"


Mat2Normals::Mat2Normals()
{
	_stride = 1;
}

Mat2Normals::~Mat2Normals()
{

}


/*
Process an image and return normals as a cv::Matrix. 
@param mat - a 2d matrix of type CV_32FC3 with surface coordinates of the object of interest. 
@param points - a 2d matrix of type CV_32FC3 with normal vectors for each non-zero point in mat. 
*/
void Mat2Normals::process(cv::Mat& mat, cv::Mat& dst_normals)
{

	int cols = mat.cols;
	int rows = mat.rows;


	dst_normals = cv::Mat::zeros(cols, rows, CV_32FC3);

	for (int i = 0; i < rows; i += _stride) {
		for (int j = 0; j < cols; j += _stride) {
			if (i < rows && j < cols) {
				cv::Vec3f p = mat.at<cv::Vec3f>(i, j);
				float sum = p[0] + p[1] + p[2];
				
				// check if p is a valid point 
				if (sum == 0.0)  continue;

				vector<cv::Vec3f> normal_vectors;
				
				cv::Vec3f p_r = cv::Vec3f(0,0,0);
				cv::Vec3f p_d = cv::Vec3f(0,0,0);
				cv::Vec3f p_l = cv::Vec3f(0,0,0);
				cv::Vec3f p_u = cv::Vec3f(0,0,0);
				bool dir[4] = { 0,0,0,0 };
				if (j + 1 < cols) {
					p_r = mat.at<cv::Vec3f>(i, j + 1) - p;
					dir[0] = true;
				}
				if (j - 1 >= 0) { 
					p_l = mat.at<cv::Vec3f>(i, j - 1) - p; 
					dir[1] = true;
				}
				if (i + 1 < rows) {
					p_d = mat.at<cv::Vec3f>(i + 1, j) - p;
					dir[2] = true;
				}
				if (i - 1 >= 0) {
					p_u = mat.at<cv::Vec3f>(i - 1, j) - p;
					dir[3] = true;
				}

				if(dir[0]&& dir[2])
					normal_vectors.push_back(cross(p_r, p_d));

				if(dir[2]&& dir[1])
					normal_vectors.push_back(cross(p_d, p_l));

				if(dir[1]&& dir[3])
					normal_vectors.push_back(cross(p_l, p_u));

				if(dir[3]&& dir[0])
					normal_vectors.push_back(cross(p_u, p_r));

				cv::Vec3f normal = cv::Vec3f(0,0,0);
				for (auto n : normal_vectors) {
					cv::Vec3f n_norm = normalize(n);
					normal += n_norm;
				}
				normal /=  float(normal_vectors.size());
				normal = normalize(normal);

				if(isnan(normal[0]) || isnan(normal[1]) || isnan(normal[2])) normal = cv::Vec3f(0,0,0);

				dst_normals.at<cv::Vec3f>(i, j) = normal;
				
			}
		}
	}

	//cv::imshow("normals", dst_normals);
	//cv::waitKey(1);
}


/*
Cross-product between a and b.
*/
cv::Vec3f Mat2Normals::cross(cv::Vec3f a, cv::Vec3f b)
{
	cv::Vec3f c;
	c[0] = a[1] * b[2] - a[2] * b[1];
	c[1] = a[2] * b[0] - a[0] * b[2];
	c[2] = a[0] * b[1] - a[1] * b[0];
	return c;
}



cv::Vec3f Mat2Normals::normalize(cv::Vec3f a)
{
	float l = length(a);
	return a / l;
}

float Mat2Normals::length(cv::Vec3f a)
{
	return sqrtf((a[0] * a[0] + a[1] * a[1] + a[2] * a[2]));
}


/*
Set the stride value for the matrix.
@param stride - integer >= 1.
*/
void Mat2Normals::setStride(int stride)
{
	if (stride > 0) {
		_stride = stride;
	}
}