#pragma once


// stl
#include <iostream>
#include <string>
#include <vector>

// einge
#include <Eigen/Dense>


class PointCloudEval
{
public:

	static float Compare(const float acceptable_delta, std::vector<Eigen::Vector3f>& points0, std::vector<Eigen::Vector3f>& normals0,
													  std::vector<Eigen::Vector3f>& points1, std::vector<Eigen::Vector3f>& normals1);
};