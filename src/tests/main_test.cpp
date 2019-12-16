
// stl
#include <iostream>
#include <string>
#include <vector>

// einge
#include <Eigen/Dense>

//local
#include "TestPointCloudGen.h"
#include "PointCloudEval.h"

#include "ReaderWriterPLY.h"
#include "ReaderWriterOBJ.h"




void run_readerwriter_test(void) {

	std::vector<Eigen::Vector3f> points;
	std::vector<Eigen::Vector3f> normals;

	std::vector<Eigen::Vector3f> points_in;
	std::vector<Eigen::Vector3f> normals_in;

	//-------------------------------------------------------------------------------
	// High number tests

	std::vector<int> test_cases = RandomNumbers::GetNumbersInt(20, 100,  400000);

	size_t N = test_cases.size();
	int count = 0;
	int count_err = 0;
	for(auto n : test_cases){
		TestPointCloudGen::Generate(n, 1000, points, normals);
		ReaderWriterPLY::Write("testcase0.ply", points, normals);
		ReaderWriterPLY::Read("testcase0.ply", points_in, normals_in);
		float err = PointCloudEval::Compare(0.001, points, normals, points_in, normals_in);

		if (err > 0.001) {
			cout << "[TEST FAILED] for test " << count << " with error = " << err << std::endl;
			count_err++;
		}else
			std::cout << "[INFO] - test " << count << " -> error = " << err << std::endl;

		count++;
	}

	std::cout << "INFO: mid level test failed in " << count_err << " cases" << std::endl;


	//-------------------------------------------------------------------------------
	// very High number tests

	test_cases = RandomNumbers::GetNumbersInt(20, 100,  400000);

	N = test_cases.size();
	count = 0;
	int count_err_high = 0;
	for(auto n : test_cases){
		TestPointCloudGen::Generate(n, 10000, points, normals);
		ReaderWriterPLY::Write("testcase0.ply", points, normals);
		ReaderWriterPLY::Read("testcase0.ply", points_in, normals_in);
		float err = PointCloudEval::Compare(0.01, points, normals, points_in, normals_in);

		if (err > 0.01) {
			cout << "[TEST FAILED] for test " << count << " with error = " << err << std::endl;
			count_err_high++;
		}else
			std::cout << "[INFO] - test " << count << " -> error = " << err << std::endl;

		count++;
	}

	std::cout << "INFO: high value test failed in " << count_err_high << " cases" << std::endl;


	//-------------------------------------------------------------------------------
	// Small number tests

	test_cases = RandomNumbers::GetNumbersInt(20, 100,  400000);

	N = test_cases.size();
	count = 0;
	int count_err_low = 0;
	for(auto n : test_cases){
		TestPointCloudGen::Generate(n, 1.0, points, normals);
		ReaderWriterPLY::Write("testcase0.ply", points, normals);
		ReaderWriterPLY::Read("testcase0.ply", points_in, normals_in);
		float err = PointCloudEval::Compare(0.0001, points, normals, points_in, normals_in);

		if (err > 0.001) {
			cout << "[TEST FAILED] for test " << count << " with error = " << err << std::endl;
			count_err_low++;
		}else
			std::cout << "[INFO] - test " << count << " -> error = " << err << std::endl;

		count++;
	}

	std::cout << "INFO: low value test failed in " << count_err_low << " cases" << std::endl;
	std::cout << "INFO: mid level test failed in " << count_err << " cases" << std::endl;
	std::cout << "INFO: high value test failed in " << count_err_high << " cases" << std::endl;
}


void run_obj_readerwriter_test(void) {

	std::vector<Eigen::Vector3f> points;
	std::vector<Eigen::Vector3f> normals;

	std::vector<Eigen::Vector3f> points_in;
	std::vector<Eigen::Vector3f> normals_in;

	//-------------------------------------------------------------------------------
	// High number tests

	std::vector<int> test_cases = RandomNumbers::GetNumbersInt(20, 100,  400000);

	size_t N = test_cases.size();
	int count = 0;
	int count_err = 0;
	for(auto n : test_cases){
		TestPointCloudGen::Generate(n, 1000, points, normals, true);
		ReaderWriterOBJ::Write("testcase0.obj", points, normals);
		ReaderWriterOBJ::Read("testcase0.obj", points_in, normals_in);
		float err = PointCloudEval::Compare(0.001, points, normals, points_in, normals_in);

		if (err > 0.001) {
			cout << "[TEST FAILED] for test " << count << " with error = " << err << std::endl;
			count_err++;
		}else
			std::cout << "[INFO] - test " << count << " -> error = " << err << std::endl;

		count++;
	}

	std::cout << "INFO: mid level test failed in " << count_err << " cases" << std::endl;


	//-------------------------------------------------------------------------------
	// very High number tests

	test_cases = RandomNumbers::GetNumbersInt(20, 100,  400000);

	N = test_cases.size();
	count = 0;
	int count_err_high = 0;
	for(auto n : test_cases){
		TestPointCloudGen::Generate(n, 10000, points, normals, true);
		ReaderWriterOBJ::Write("testcase0.obj", points, normals);
		ReaderWriterOBJ::Read("testcase0.obj", points_in, normals_in);
		float err = PointCloudEval::Compare(0.01, points, normals, points_in, normals_in);

		if (err > 0.01) {
			cout << "[TEST FAILED] for test " << count << " with error = " << err << std::endl;
			count_err_high++;
		}else
			std::cout << "[INFO] - test " << count << " -> error = " << err << std::endl;

		count++;
	}

	std::cout << "INFO: high value test failed in " << count_err_high << " cases" << std::endl;


	//-------------------------------------------------------------------------------
	// Small number tests

	test_cases = RandomNumbers::GetNumbersInt(20, 100,  400000);

	N = test_cases.size();
	count = 0;
	int count_err_low = 0;
	for(auto n : test_cases){
		TestPointCloudGen::Generate(n, 1.0, points, normals, true);
		ReaderWriterOBJ::Write("testcase0.obj", points, normals);
		ReaderWriterOBJ::Read("testcase0.obj", points_in, normals_in);
		float err = PointCloudEval::Compare(0.0001, points, normals, points_in, normals_in);

		if (err > 0.001) {
			cout << "[TEST FAILED] for test " << count << " with error = " << err << std::endl;
			count_err_low++;
		}else
			std::cout << "[INFO] - test " << count << " -> error = " << err << std::endl;

		count++;
	}

	std::cout << "INFO: low value test failed in " << count_err_low << " cases" << std::endl;
	std::cout << "INFO: mid level test failed in " << count_err << " cases" << std::endl;
	std::cout << "INFO: high value test failed in " << count_err_high << " cases" << std::endl;
}


int main(int argc, char* argv[])
{

	//run_readerwriter_test();
	run_obj_readerwriter_test();


	return 1;
}