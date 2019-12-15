#pragma once
/*
class GeometryCheckup

The class provides several features to check the geometry of the object before processing it

Features:
- Load and write  temporary obj files: the class works with temporary obj files. It reads the original file, 
  rewrites the content (centorid, material), and saves the new geometry in a new file. 
- Create material file: it creates a new material file in the case the given object does not come with any material.
- Calculate the geometric centroid: it calculates the geometric centroid of an object as its mean.
- Calculate bounding box: the class calculates the bounding box and the edge length of the object.
- Move the object to bb center: it modifices all vertices so that the object ends up in the bb centroid. 
- Checks the possible density for the point cloud sampler: with the bb size at hand, the class determines the 
  minimum possible density so that the application does not stall. It assumes that the comptuer has sufficient 
  resources to handle 1000 voxels in each direction. 

Rafael Radkowski
Iowa State University
rafael@iastate.edu
March 2019
MIT License
------------------------------------------------------------------------------------------------------
Last edits:


*/

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <cassert>
#ifdef _MSC_VER <= 1916
#include <experimental/filesystem>
#else
#include <filesystem>
#endif
#ifdef __WIN32
	#include <conio.h>
#endif

// eigen
#include <Eigen/Dense>

// local
#include "Types.h"
#include "LoaderOBJ.h"



class GeometryCheckup {

public:
	GeometryCheckup();
	~GeometryCheckup();


	/*
	Center the obj model to its centroid and calculate the bouding box parameters
	@param input_path_and_file -  string containg the path and the file. MUST BE AN OBJ file
	@param output_path_and_file - string containg the output path and the file. MUST BE AN OBJ file
	*/
	bool processObj(std::string input_path_and_file, std::string output_path_and_file);



	/*!
	Load an obj file. 
	@param path_and_file - string containg the path and the file. 
	@return true if the object was successfully loaded
	*/
	bool loadObj(std::string path_and_file);


	/*
	Write the obj data to a file.
	@param path_and_file - string containg the path and the file. 
	*/
	bool writeObj(std::string path_and_file);


	/*
	Identify the centroid of the object. 
	@param centroid - location to store the coordinates of the centroid
	*/
	bool calculateGeometricCentroid(Eigen::Vector3f& centroid);

	/*
	Move the entire dataset to the center of the bouding box
	@param centroid - coordinates of the centroid
	*/
	bool setToBoundingBoxCenter(void);

	/*
	Calculate the bouding box of the given object. 
	@param boundingbox_size - location to store the dimensions
	*/
	bool calculateBoundingBox( Eigen::Vector3f& boundingbox_size);

	/*
	Enable or disable outputs
	*/
	bool setVerbose(bool verbose);


	/*
	Return the largest bounding box edge
	*/
	float getMaxBBEdge(void);


	/*
	The function computes the minimum recommended 
	voxel density for any given object. 
	*/
	float computeMinVoxelDensity(void);

	

private:

	/*
	Reset all internal data. 
	*/
	bool reset(void);

	// Check if a value is a number
	bool is_number(std::string s);

	// split the line into its elements
	std::vector<std::string> split(const std::string& s, char delimiter);

	//check the material file and write a temporary material file out if non exists.
	// @param path_and_file_3d_model - the path and filename of the user 3D model
	// @param output_path_and_file - the new material file, path and filename;
	bool processMtlFile(std::string path_and_file_3d_model, std::string output_path_and_file);


	//check the material file and write a temporary material file out if non exists.
	// @param path_and_file - the original material file
	// @param output_path_and_file - the new material file, path and filename;
	bool writeMaterialFile(std::string path_and_file, std::string output_path_and_file);

	////////////////////////////////////////////////////////////////////////////

	// content of the loaded file
	std::vector<string>		_original_file_content;

	// the orignal points and normal vectors
	std::vector<Eigen::Vector3f>	_points;
	std::vector<Eigen::Vector3f>	_normals;


	// line that contains the first valid data set, v, or vn. 
	int						_first_line_data;					
	
	// path and file - original content
	std::string				_path_and_file;

	// the material library path
	std::string				_mtllib_file;
	int						_mtl_line;
	
	// the new material lib name which is used for the output file.
	// It is extracted from the new filename + .mtl
	std::string				_new_mtllib_file;



	// store the used materials. 
	std::vector<std::string> _usemtl_list;

	// the objects centroid
	float					_centroid[3];
	float					_bounding_box[3];
	float					_max_size;
	
	// get output
	bool					_verbose;


	float maxX;
    float maxY;
    float maxZ;
    float minX;
    float minY;
    float minZ;
};