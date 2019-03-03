#ifndef __LOADER_OBJ__
#define __LOADER_OBJ__


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
	@param dst_normals - vector of vector3f normal vectors index-aligned to the points
	*/
	static bool Write(string file, vector<Eigen::Vector3f>* dst_points, vector<Eigen::Vector3f>* dst_normals);
};

#endif