#pragma once
/*
class Mat2Points
The class takes a matrix with surface points and copies all points into a vector.

Rafael Radkowski
Iowa State University
rafael@iastate.edu
Jan 2019
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


// local
#include "Mat2Normals.h"


using namespace std;


class Mat2Points {

public:

	Mat2Points();
	~Mat2Points();


	/*
	Process an image and return points as vector.
	@param mat - a 2d matrix with surface coordinates of the object of interest. 
	@param points - a pointer to a vector to store the points as (x, y, z). 
	@param normals - a pointer to a vector to store the normals as (x, y, z). 
	*/
	void process(cv::Mat& mat, std::vector<Eigen::Vector3f>* points, std::vector<Eigen::Vector3f>* normals);


	/*
	Set the stride value for the matrix.
	@param stride - integer >= 1.
	*/
	void setStride(int stride);


private:

	int			_stride;


	Mat2Normals	_nc;


};