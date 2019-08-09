/*

Rafael Radkowski
Iowa State University
rafael@iastate.edu
+1 (515) 294 7044
March 2019
MIT License

---------------------------------------------------
Last edits:

*/

#include <iostream>
#include <string>
#include <time.h>


// opencv
#include <opencv2/opencv.hpp>


// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>  // transformation
#include <glm/gtx/quaternion.hpp> // quaternions

// glfw includes
#include <GLFW/glfw3.h>


// local
#include "SurfExtractApp.h"
#include "ArgParser.h"


int						_image_width = 1024; 
int						_image_height = 1024;
int						_window_width = 2048; 
int						_window_height = 1536;


using namespace std;

int main(int argc, char** argv) {


	arlab::Arguments arg = arlab::ArgParser::Parse(argc,argv);

	if (!arg.valid) {
		arlab::ArgParser::Help();
		return 1;
	}

	cout << "SurfExtract" << endl;
	cout << "Version 1.1" << endl;
	cout << "Extract a point cloud from all visible surfaces and ignore hidden surfaces\n" << endl;
	cout << "Rafael Radkowski" << endl;
	cout << "rafael@iastate.edu" << endl;
	cout << "Iowa State University" << endl;
	cout << "August 2019" << endl;
	cout << "MIT License\n\n" << endl;


	arlab::ArgParser::Display();

	cout << "------------------------------------------------------------------------" << endl;
	cout << "Keyboard commands, use" << endl;
	cout << ", \t to increase the camera distance" << endl;
	cout << ". \t to decrease the camera distance" << endl;
	cout << "r \t to resample the point cloud with a different voxel density" << endl;
	cout << "s \t to re-save the point cloud" << endl;
	cout << "1 \t enable/disable 3D model rendering" << endl;
	cout << "2 \t enable/disable point cloud rendering" << endl;
	cout << "3 \t enable/disable normal vector rendering (after all points are done)" << endl;
	cout << "------------------------------------------------------------------------" << endl;
	cout << "IGNORE THE MATERIAL WARNIGS\n\n" << endl;


	SurfExtractApp* sea = new SurfExtractApp();
	sea->loadModel(arg.model_path_and_file);
	sea->setCameraDistance(arg.camera_distance);
	sea->setPointCloudDensity(arg.voxel_grid_size);
	sea->setVerbose(arg.verbose);
	sea->setOutputScale(arg.output_scale);
	sea->setOutputFilename(arg.output_path_filename);

	sea->start();


	return 1;

}