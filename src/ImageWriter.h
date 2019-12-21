#ifndef __IMAGEWRITER__
#define __IMAGEWRITER__
/*
class ImageWriter

The class OpenCV Mat images to .png files as well as the pose of an object as matrix to a .txt file.
It also maintains a log file to summarize all writing tasks. The log file is overwritten each time an instance of
this class is created. 

Features:


Usage:


Rafael Radkowski
Iowa State University
rafael@iastate.edu
+1 (515) 294 7044
Jan 2019
All copyrights reserved

------------------------------------------------------------------
last edited:
Jan 21, 2019:
- Added the pose and orientation of the model to the log file
- Added a variable for the log file to the class
- Fixed a bug that prevented that the class cleans the output folder. 
Dec 21, 2019
- Added FileUtils.h and replaced the c++ api with fileutils apis. 
*/

// stl
#include <iostream>
#include <vector>
#include <string>
#if _MSC_VER >= 1920 && _MSVC_LANG  == 201703L 
#include <filesystem>
#else
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <experimental/filesystem>
#define _USE_EXP
#endif

// opencv
#include <opencv2/opencv.hpp>

// Eigen
#include <Eigen/Dense>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>  // transformation
#include <glm/gtx/quaternion.hpp> // quaternions

// local
#include "MatrixFileUtils.h"
#include "MatrixHelpers.h"
#include "types.h"
#include "FileUtils.h"

using namespace std;

class ImageWriter
{
public:

	ImageWriter();
	~ImageWriter();


	/*
	Set the output path and name
	@param path - the output file path
	@param name - the file name as string
	*/
	void setPathAndImageName(string path, string name);


	/*
	Write the image data to a file
	@param index - the image id;
	@param rgb - cv::Mat& of type CV_8U_C3 with the rbg image content
	@param normals - cv::Mat of type CV_32F_C3 with normal vectors 
	@param depth - cv::Mat of type CV_32F_C1 with the depth values
	@para pose - the current camera pose
	*/
	bool write(int index, cv::Mat& rgb, cv::Mat& normals, cv::Mat& depth, glm::mat4 pose);

	/*
	Write the image data to a file
	@param index - the image id;
	@param image - cv::Mat of type CV_32F_C3 with normal vectors 
	@para pose - the current camera pose
	*/
	bool write_32FC3(int index, cv::Mat& image, glm::mat4 pose);

private:

	/*
	Check whether the path exists. 
	Create a folder if the path does not exist. 
	@param path - string with the relative or absolute path. 
	*/
	bool checkFolder(string path);


	//---------------------------------------------------------
	// members

	string					_output_file_path;
	string					_output_file_name;

	string					_logfile_name;



};


#endif