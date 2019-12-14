#ifndef __LOADER_OBJ__
#define __LOADER_OBJ__

/*


---------------------------------------------------
Last edits:

Dec 14, 2019, RR:
- Set the output to a fixed length to prevent scientific notation. Some other applications
  crash with the e as part of the number.

*/


#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#ifdef __WIN32
	#include <conio.h>
#endif

// eigen
#include <Eigen/Dense>

// local
#include "Types.h"

using namespace std;


class LoaderObj{
private:

	//Helper functions
	static bool is_number(std::string s);

public:

	/*!
	Load a point cloud .obj object from file
	@param file = The file
	@param loadedNormals = The output location of the loaded normals
	@return cloud = The output of the loaded point cloud object
	*/
	static bool Read(string file, vector<Eigen::Vector3f>* dst_points, vector<Eigen::Vector3f>* dst_normals, bool invert_z = true);


	/*
	Write the point cloud data to a file
	@param file - string containing path and name
	@param dst_points - vector of vector3f points containing x, y, z coordinates
	@param dst_normals - vector of vector3f normal vectors index-aligned to the points.
	@param scale_points - float value > 0.0 that scales all points and normal vectors. 
	*/
	static bool Write(string file, vector<Eigen::Vector3f>* dst_points, vector<Eigen::Vector3f>* dst_normals, float scale_points = 1.0f);
};

#endif