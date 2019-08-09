#pragma once
/*
class SurfExtractAppEx

Extension for the GUI version



Rafael Radkowski
Iowa State University
rafael@iastate.edu
+1 (515) 294 7044
April 14, 2019
MIT License

---------------------------------------------------
Last edits:

*/

#include <iostream>
#include <string>
#include <time.h>
#include <thread>


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

using namespace std::placeholders;
using namespace std;

class SurfExtractAppEx : public SurfExtractApp
{

public:

	SurfExtractAppEx();
	~SurfExtractAppEx();


	/*
	Start the opengl interface. 
	*/
	bool startGUI(void);


	/*
	Load the 3D model from a file
	*/
	bool loadModelEx(string path_to_file);

private:

	// the 3d obj model
	string	_model_path_to_file; 

};