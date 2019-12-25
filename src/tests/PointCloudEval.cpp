#include "PointCloudEval.h"



//static 
float PointCloudEval::Compare(const float acceptable_delta, std::vector<Eigen::Vector3f>& points0, std::vector<Eigen::Vector3f>& normals0,
															std::vector<Eigen::Vector3f>& points1, std::vector<Eigen::Vector3f>& normals1)
{

	float error = 0.0f;

	int size = points0.size();
	if (normals0.size() != size || points1.size() != size || normals1.size() != size) {
		std::cout << "[TEST FAILED] - point cloud sizes do not match: normals0.size() != size || points1.size() != size || normals1.size() != size" << std::endl;
		return error;
	}


	for (int i = 0; i < size; i++) {
		Eigen::Vector3f p0 = points0[i];
		Eigen::Vector3f n0 = normals0[i];
		Eigen::Vector3f p1 = points1[i];
		Eigen::Vector3f n1 = normals1[i];

		float delta_p =  (p0 - p1).stableNorm();
		float delta_n =  (n0 - n1).stableNorm();

		/*if (delta_p > acceptable_delta) {
			std::cout << "[TEST FAILED] - point at index " << i << " with error " << delta_p << std::endl;
		}

		if (delta_n > acceptable_delta) {
			std::cout << "[TEST FAILED] - normal at index " << i << " with error " << delta_p << std::endl;
		}*/

		error += delta_p;
		error += delta_n;

	}

	error /= (size*2.0);

	return error;
}