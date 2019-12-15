#include "ReaderWriterPCD.h"



	/*!
Load a point cloud object from a file
@param file - The file
@param loadedNormals = The output location of the loaded normals
@return cloud = The output of the loaded point cloud object
*/
//virtual 
//static 
bool ReaderWriterPCD::Read(const std::string file, std::vector<Eigen::Vector3f>& dst_points, std::vector<Eigen::Vector3f>& dst_normals, const bool invert_z)
{


	return true;
}


/*
Write the point cloud data to a file
@param file - string containing path and name
@param dst_points - vector of vector3f points containing x, y, z coordinates
@param dst_normals - vector of vector3f normal vectors index-aligned to the points.
@param scale_points - float value > 0.0 that scales all points and normal vectors. 
*/
//virtual 
//static 
bool ReaderWriterPCD::Write(std::string file, std::vector<Eigen::Vector3f>& src_points, std::vector<Eigen::Vector3f>& src_normals, const float scale_points)
{
	
	// check if the number of points matches the number of normals
	if (src_points.size() != src_normals.size()) {
		std::cout << "[ERROR] - ReaderWriterPCD: number of points and normals does not match: " << src_points.size() << " != " << src_normals.size() << std::endl;
		return false;
	}


	// append a pcd ending 
	int index = file.find_last_of(".");
	std::string outfile;

	if (index != -1)
	{
		outfile = file.substr(0, index);
	}
	outfile.append(".pcd");


	std::ofstream of;
	of.open(outfile, std::ofstream::out);

	size_t size = src_points.size();

	if (!of.is_open()) {
		std::cout << "[ERROR] - ReaderWriterPCD: cannot open file " << outfile << " for writing." << std::endl;
		return false;
	}
	

	of << "# .PCD v0.7 - Point Cloud Data file format\n";
	of << "VERSION 0.7\n";
	of << "FIELDS x y z normal_x normal_y normal_z\n";
	of << "SIZE 4 4 4 4 4 4\n";
	of << "TYPE F F F F F F\n";
	of << "COUNT 1 1 1 1 1 1\n";
	of << "WIDTH " << std::to_string(size) << "\n";
	of << "HEIGHT 1\n";
	of << "VIEWPOINT 0 0 0 1 0 0 0\n";
	of << "POINTS " << std::to_string(size) << "\n";
	of << "DATA ascii\n";


	for (int i = 0; i < size; i++) {

		Eigen::Vector3f p = src_points[i];
		Eigen::Vector3f n = src_normals[i];
				
		if (p[0] || p[1] || p[2]) {
			of << std::fixed << scale_points * p.x() << " " << scale_points * p.y() << " " << scale_points * p.z() << " "  << n.x() << " " << n.y() << " " << n.z() << "\n";
		}
	}
	of.close();
	

	std::cout << "[INFO] - ReaderWriterPCD: saved " << size << " points and normal vectors to file " << outfile << "." << std::endl;

	return true;
}
