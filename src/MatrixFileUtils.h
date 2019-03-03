#pragma once

#include <iostream>
#include <fstream>
#include <experimental\filesystem>
#include <conio.h>

// Eigen 3
#include <Eigen\Dense>

using namespace std;
using namespace Eigen;

class MatrixFileUtils
{
public:



	/*
	Read a Eigen matrix from a file
	*/
	static Eigen::Matrix4f ReadMatrix4f(string path_and_file);

	/*
	Write a matrix to a file
	*/
	static void WriteMatrix4f(string path_and_file, Eigen::Matrix4f matrix, string label = "matrix:");
	

	static void WriteMatrix3f(string path_and_file, Eigen::Matrix3f matrix, string label = "matrix:");
	static Eigen::Matrix3f ReadMatrix3f(string path_and_file);


	static void WriteVectorXf(string path_and_file, Eigen::VectorXf vector, string label = "vector:");
	static Eigen::VectorXf ReadVectorXf(string path_and_file);


};

