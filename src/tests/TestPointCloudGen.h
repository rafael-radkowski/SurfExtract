#pragma once


// stl
#include <iostream>
#include <string>
#include <vector>

// einge
#include <Eigen/Dense>

//local
#include "RandomNumbers.h"



class TestPointCloudGen
{
public:

	static bool Generate(const int num_points, const float magnitute, std::vector<Eigen::Vector3f>& dst_points, std::vector<Eigen::Vector3f>& dst_normals );


};