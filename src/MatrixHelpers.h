#pragma once


// stl
#include <iostream>
#include <vector>
#include <conio.h>
#include <experimental\filesystem>

#include <Windows.h> // for file attributes
#include <cassert>

// Eigen
#include "Eigen\Dense"





class MatrixHelpers
{

public:

	/*
	Convert a matrix into a quaternion
	From http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
	but adopted to Eigen matrices and quaternions. 
	*/
	static inline void MatrixToQuaternion(Eigen::Matrix4f a, Eigen::Quaternionf& q) {
		

		Matrix3f mat = a.block(0, 0, 3, 3);
		Quaternionf c(mat);
		q = c;

		return;




		float trace = a(0,0) + a(1,1) + a(2,2); // I removed + 1.0f; see discussion with Ethan
		if (trace > 0) {// I changed M_EPSILON to 0
			float s = 0.5f / sqrtf(trace + 1.0f);
			q.w() = 0.25f / s;
			q.x() = (a(2,1) - a(1,2)) * s;
			q.y() = (a(0,2) - a(2,0)) * s;
			q.z() = (a(1,0) - a(0,1)) * s;
		}
		else {
			if (a(0,0) > a(1,1) && a(0,0) > a(2,2)) {
				float s = 2.0f * sqrtf(1.0f + a(0,0) - a(1,1) - a(2,2));
				q.w() = (a(2,1) - a(1,2)) / s;
				q.x() = 0.25f * s;
				q.y() = (a(0,1) + a(1,0)) / s;
				q.z() = (a(0,2) + a(2,0)) / s;
			}
			else if (a(1,1) > a(2,2)) {
				float s = 2.0f * sqrtf(1.0f + a(1,1) - a(0,0) - a(2,2));
				q.w() = (a(0,2) - a(2,0)) / s;
				q.x() = (a(0,1) + a(1,0)) / s;
				q.y() = 0.25f * s;
				q.z() = (a(1,2) + a(2,1)) / s;
			}
			else {
				float s = 2.0f * sqrtf(1.0f + a(2,2) - a(0,0) - a(1,1));
				q.w() = (a(1,0) - a(0,1)) / s;
				q.x() = (a(0,2) + a(2,0)) / s;
				q.y() = (a(1,2) + a(2,1)) / s;
				q.z() = 0.25f * s;
			}
		}
	}




	/*
	Convert a quaternion into a matrix. 
	From http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
	and adapted for Eigen.
	*/
	static inline void QuaternionToMatrix(Eigen::Quaternionf q, Eigen::Matrix4f& a) {

		double sqw = q.w()*q.w();
		double sqx = q.x()*q.x();
		double sqy = q.y()*q.y();
		double sqz = q.z()*q.z();

		// invs (inverse square length) is only required if quaternion is not already normalised
		double invs = 1 / (sqx + sqy + sqz + sqw);
		a(0,0) = (sqx - sqy - sqz + sqw)*invs; // since sqw + sqx + sqy + sqz =1/invs*invs
		a(1,1) = (-sqx + sqy - sqz + sqw)*invs;
		a(2,2) = (-sqx - sqy + sqz + sqw)*invs;

		double tmp1 = q.x()*q.y();
		double tmp2 = q.z()*q.w();
		a(1,0) = 2.0 * (tmp1 + tmp2)*invs;
		a(0,1) = 2.0 * (tmp1 - tmp2)*invs;

		tmp1 = q.x()*q.z();
		tmp2 = q.y()*q.w();
		a(2,0) = 2.0 * (tmp1 - tmp2)*invs;
		a(0,2) = 2.0 * (tmp1 + tmp2)*invs;
		tmp1 = q.y()*q.z();
		tmp2 = q.x()*q.w();
		a(2,1) = 2.0 * (tmp1 + tmp2)*invs;
		a(1,2) = 2.0 * (tmp1 - tmp2)*invs;

		a(3, 0) = 0;
		a(3, 1) = 0;
		a(3, 2) = 0;
		a(3, 3) = 1;

		a(0, 3) = 0;
		a(1, 3) = 0;
		a(2, 3) = 0;
		
	}





};