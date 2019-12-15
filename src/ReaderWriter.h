#pragma once
/*
Class ReaderWriter
This is an abstract class for all ReaderWriter classes. 

Rafael Radkowski
Iowa State University
rafael@iastate.edu
December 2019
MIT License
------------------------------------------------------------------------------------------------------
Last edits:


*/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <strstream>

// Eigen
#include <Eigen/Dense>

class ReaderWriter
{
public:


	/*!
	Load a point cloud object from a file
	@param file - The file
	@param loadedNormals = The output location of the loaded normals
	@return cloud = The output of the loaded point cloud object
	*/
	virtual bool Read(const std::string file, std::vector<Eigen::Vector3f>& dst_points, std::vector<Eigen::Vector3f>& dst_normals, const bool invert_z = true) = 0;


	/*
	Write the point cloud data to a file
	@param file - string containing path and name
	@param dst_points - vector of vector3f points containing x, y, z coordinates
	@param dst_normals - vector of vector3f normal vectors index-aligned to the points.
	@param scale_points - float value > 0.0 that scales all points and normal vectors. 
	*/
	virtual bool Write(std::string file, std::vector<Eigen::Vector3f>& dst_points, std::vector<Eigen::Vector3f>& dst_normals, const float scale_points = 1.0f) = 0;


protected:

	//Helper functions to check whether the string character is a number
	static bool is_number(std::string s) {
		 return !s.empty() && s.find_first_not_of("0123456789.-e") == std::string::npos;
	}

	// check the expected filetype of the object
	static bool check_type(std::string path_and_file, std::string type) {

	}

	// split the lines into single elements
	static  std::vector<std::string> split(const std::string& s, char delimiter)
	{
	   std::vector<std::string> tokens;
	   std::string token;
	   std::istringstream tokenStream(s);
	   while (std::getline(tokenStream, token, delimiter))
	   {
		  tokens.push_back(token);
	   }
	   return tokens;
	}

};