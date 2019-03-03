#pragma once



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


class Mat2Points {

public:

	Mat2Points();
	~Mat2Points();


	/*
	Process an image and return points in 
	*/
	void process(cv::Mat& mat, std::vector<Eigen::Vector3f>* points);


	/*
	Set the stride value for the matrix.
	@param stride - integer >= 1.
	*/
	void setStride(int stride);


private:

	int			_stride;


};