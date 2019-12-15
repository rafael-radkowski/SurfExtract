#include "TestPointCloudGen.h"


//static 
bool TestPointCloudGen::Generate(const int num_points, const float magnitute, std::vector<Eigen::Vector3f>& dst_points, std::vector<Eigen::Vector3f>& dst_normals )
{
	dst_points.clear();
	dst_normals.clear();

	dst_points.reserve(num_points);
	dst_normals.reserve(num_points);

	vector<float> p = RandomNumbers::GetNumbers(num_points*3, -magnitute, magnitute);
	vector<float> n = RandomNumbers::GetNumbers(num_points*3, -magnitute, magnitute);

	for (int i = 0; i < num_points; i++) {
		
		dst_points.push_back(Eigen::Vector3f(p[i*3], p[i*3 + 1], p[i*3 + 2]));
		dst_normals.push_back(Eigen::Vector3f(n[i*3], n[i*3 + 1], n[i*3 + 2]));
	}

	return true;
}