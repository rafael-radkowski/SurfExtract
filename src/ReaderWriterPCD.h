#pragma once
/*
Class ReaderWriterPCD
This class reads and writes PCD files

Rafael Radkowski
Iowa State University
rafael@iastate.edu
December 2019
MIT License
------------------------------------------------------------------------------------------------------
Last edits:


*/
// stl
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <strstream>

// Eigen
#include <Eigen/Dense>

// local
#include "ReaderWriter.h"


class ReaderWriterPCD : public ReaderWriter
{
public:

		/*!
	Load a point cloud object from a file
	@param file - The file
	@param loadedNormals = The output location of the loaded normals
	@return cloud = The output of the loaded point cloud object
	*/
	//virtual 
	static bool Read(const std::string file, std::vector<Eigen::Vector3f>& dst_points, std::vector<Eigen::Vector3f>& dst_normals, const bool invert_z = true);


	/*
	Write the point cloud data to a file
	@param file - string containing path and name
	@param dst_points - vector of vector3f points containing x, y, z coordinates
	@param dst_normals - vector of vector3f normal vectors index-aligned to the points.
	@param scale_points - float value > 0.0 that scales all points and normal vectors. 
	*/
	//virtual 
	static bool Write(std::string file, std::vector<Eigen::Vector3f>& src_points, std::vector<Eigen::Vector3f>& src_normals, const float scale_points = 1.0f);


private:


};
