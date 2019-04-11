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
#include "GLRenderer.h"
#include "ModelOBJ.h"
#include "PolyhedronViewRenderer.h"
#include "PointCloudAssembly.h"
#include "GLPointCloudRenderer.h"
#include "GLNormalsRenderer.h"

using namespace std::placeholders;
using namespace std;


class SurfExtractApp {

public:

	SurfExtractApp();
	~SurfExtractApp();

	/*
	Load the 3D model from a file
	*/
	bool loadModel(string path_to_file);

	/*
	Set the prerenderer camera distanct
	@param distance - the distance as float
	*/
	void setCameraDistance(float distance);


	/*
	Set the density of the point cloud
	@param density - the point cloud density as voxel cell distance.
	Must be >= 0.005
	*/
	void setPointCloudDensity(float density);

	/*
	Resample an existing point cloud
	@param density - the point cloud density as voxel cell distance.
	Must be >= 0.005
	*/
	void resample(float density);


	/*
	Save the point cloud to an  obj file.
	@param path - string containing the path and file. 
	Ending must be obj.
	*/
	bool saveToObj(string path);

	/*
	Save the point cloud to an  obj file once the procedure is complete
	@param path - string containing the path and file. 
	Ending must be obj.
	*/
	void setOutputFilename(string path);

	/*
	Enable model rendering
	*/
	void show3DModel(bool show) { _enable_render_3d = show; }

	/*
	Enable point cloud rendering
	*/
	void showPointCloud(bool show) { _enable_render_points = show; }


	/*
	Make the application verbose
	*/
	bool setVerbose(bool verbose);

	/*
	Start the point cloud generation
	*/
	bool start(void);

	/*
	Return whether the renderer is done 
	drawing all images for the surface sequence. 
	@return false - if not done, otherwise true
	*/
	bool done(void);


private:

	/*
	To be passed to the renderer to draw the content. 
	*/
	void render_fcn(glm::mat4 proj_matrix, glm::mat4 view_matrix);

	/*
	Keyboard callback for the renderer
	*/
	void keyboard_cb(int key, int action);

	//------------------------------------------------------
	// members

	// opengl main renderer
	GLRenderer*			_renderer;

	// 3d model of the object to be rendered
	cs557::OBJModel*		_model;

	//  light for the model
	cs557::LightSource	_light0;
	cs557::LightSource	_light1;

	// image and window width / height
	int						_image_width; 
	int						_image_height;
	int						_window_width; 
	int						_window_height;

	// renders the camera views into opencv matrices
	PolyhedronViewRenderer* _pview;
	bool					_pview_complete; // indicates that all images have been rendered
	bool					_pview_start; // indicates that the model was created and that the renders is ready to go

	float					_pview_camera_distance;
	int						_pview_sub;

	// point cloud assembly tool
	PointCloudAssembly*		_pca;

	// point cloud opengl renderer
	GLPointCloudRenderer*	_gl_pc;
	GLNormalsRenderer*		_gl_normals;

	bool						_enable_render_3d;
	bool						_enable_render_points;
	bool						_enable_render_normals; // to enable the normal vector renderer automatically after all points are done
	bool						_show_normals; // to allow the user to enable and disable the normal renderer
	bool						_verbose;

	bool					_output_done;
	string					_output_path;



};