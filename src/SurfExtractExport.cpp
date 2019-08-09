#pragma once
/*
class SurfExtractApp 


Rafael Radkowski
Iowa State University
rafael@iastate.edu
+1 (515) 294 7044
March 4, 2019
MIT License

---------------------------------------------------
Last edits:

*/

#include <iostream>
#include <string>
#include <time.h>
#include <thread>


// local
#include "SurfExtractAppEx.h"
#include "ArgParser.h"


using namespace std::placeholders;
using namespace std;


namespace nsSurfExtractExport {

	SurfExtractAppEx* sea = NULL; 
};


using namespace nsSurfExtractExport;
using namespace std;

extern "C"
{
	__declspec(dllexport) int __stdcall Init(void) {
   
		sea = new SurfExtractAppEx();
		if (sea) {
			return 1;
		}
		return 0;
	}


	__declspec(dllexport) int __stdcall LoadModel(char* path_and_file, int len) {
   
		string file(path_and_file);

		if (sea) {
			sea->loadModelEx(file);
			return 1;
		}
		return 7;
	}

	__declspec(dllexport) int __stdcall SetPointCloudDensity(float point_cloud_density) {
   
		if (sea) {
			sea->setPointCloudDensity(point_cloud_density);
			return 1;
		}
		return 0;
	}

	__declspec(dllexport) int __stdcall SetCameraDistance(float cam_distance) {
   
		if (sea) {
			sea->setCameraDistance(cam_distance);
			return 1;
		}
		return 0;
	}


	__declspec(dllexport) int __stdcall SetVerbose(bool verbose) {
   
		if (sea) {
			sea->setVerbose(verbose);
			return 1;
		}
		return 0;
	}


	__declspec(dllexport) int __stdcall SetOutputScale(float output_scale) {
   
		if (sea) {
			sea->setOutputScale(output_scale);
			return 1;
		}
		return 0;
	}

	__declspec(dllexport) int __stdcall SetOutputFilename(std::string output_path_filename) {
   
		if (sea) {
			sea->setOutputFilename(output_path_filename);
			return 1;
		}
		return 0;
	}


	__declspec(dllexport) int __stdcall Start(void) {
   
		if (sea) {
			sea->startGUI();
			return 1;
		}
		return 0;
	}

	__declspec(dllexport) int __stdcall Exit(void) {
   
		if (sea) {
			delete sea;
		}
		return 1;
	}

}