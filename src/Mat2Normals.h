#pragma once
/*
class Mat2Normals

The class computes normal vectors from a matrix with (x,y,z) surface coordinates.
The class expects that all surface points come as CV_32FC3 vectors, with the channle C3 associated to the point coordinates. 
The normal vector is calculated using the cross-product to adjacent points. 

Rafael Radkowski
Iowa State University
rafael@iastate.edu
MIT License.
*/

#include <iostream>
#include <string>
#include <time.h>


// opencv
#include <opencv2/opencv.hpp>

#include <Eigen/Dense>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>  // transformation
#include <glm/gtx/quaternion.hpp> // quaternions


using namespace std;

class Mat2Normals
{
public:

	Mat2Normals();
	~Mat2Normals();


	/*
	Process an image and return normals as a cv::Matrix. 
	@param mat - a 2d matrix of type CV_32FC3 with surface coordinates of the object of interest. 
	@param points - a 2d matrix of type CV_32FC3 with normal vectors for each non-zero point in mat. 
	*/
	void process(cv::Mat& mat, cv::Mat& dst_normals);


	/*
	Set the stride value for the matrix.
	@param stride - integer >= 1.
	*/
	void setStride(int stride);

private:

	/*
	Cross-product between a and b.
	*/
	cv::Vec3f cross(cv::Vec3f a, cv::Vec3f b);


	cv::Vec3f normalize(cv::Vec3f a);

	float length(cv::Vec3f a);


	int			_stride;
};