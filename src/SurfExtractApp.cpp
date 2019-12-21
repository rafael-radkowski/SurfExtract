#include "SurfExtractApp.h"



SurfExtractApp::SurfExtractApp()
{
	_image_width = 1024; 
	_image_height = 1024;
	_window_width = 1024; 
	_window_height = 768;
	_pview_complete = false;
	_pview_start = false;
	_verbose = false;
	_enable_render_3d = true;
	_enable_render_points = true;
	_enable_render_normals = false;
	_show_normals = false;
	_pview_camera_distance = 3.5;
	_pview_camera_distance_user_value = false;
	_pview_sub = 1;

	_temp_model_path = "temp_model.obj";

	_pca = NULL;
	_gl_pc = NULL;
	_model = NULL;
	_output_path = "";
	_output_done = false;

	_renderer = new GLRenderer();
	_renderer->create(_window_width, _window_height, "Surface Extraction");
	_renderer->addRenderFcn(std::bind(&SurfExtractApp::render_fcn, this, _1, _2));
	_renderer->addKeyboardCallback(std::bind(&SurfExtractApp::keyboard_cb, this, _1, _2));

	_pview = new surfe::PolyhedronViewRenderer(_window_width, _window_height, 1024, 1024);
	_pview->setVerbose(false); // set first to get all the output info
	_pview->setOutputPath("temp");

	_pca = new PointCloudAssembly();
	_pca->setPointCloudDensity(0.025);

	// point cloud renderer
	_gl_pc = new GLPointCloudRenderer(_pca->getPointCloud().points, _pca->getPointCloud().points);
	_gl_normals = new GLNormalsRenderer(_pca->getPointCloud().points, _pca->getPointCloud().normals);
	

	
}


SurfExtractApp::~SurfExtractApp()
{
	

	delete _gl_pc;
	delete _pca;
	delete _pview;
	if (_model) delete _model;
	delete _renderer;
}


/*
Load the 3D model from a file
*/
bool SurfExtractApp::loadModel(string path_to_file)
{
	
	// precheck the model
	_geometry_check.setVerbose(_verbose);
	bool ret = _geometry_check.loadObj(path_to_file);
	if (!ret) {
		return false;
	}


	// the geometry object writes its own model to a file in _temp_model_path.
	// the tools uses this new temp model to extract the surface point cloud. 
	ret = _geometry_check.processObj(path_to_file, _temp_model_path);
	if (!ret) {
		cout << "[CRITICAL ERROR] - Something went wrong when pre-processing the model " << path_to_file << "." << endl;
		return false;
	}

	// set the maximum camera distance. Note that the user can overwrite this value.
	autoSetCameraDistance();

	// set the minimum point cloud density
	// The function calculates the minimum density that will prevent crashing.
	_pca->setMinimumPointCloudDensity(_geometry_check.computeMinVoxelDensity());
	

	// load the 3d model
	// This model is only set to have a visual model on display. 
	unsigned int program = cs557::LoadAndCreateShaderProgram("lit_scene.vs", "lit_scene.fs");
	_model = new cs557::OBJModel();
	_model->create(_temp_model_path, program);
	_model->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	_light0.pos = glm::vec3(0.0f, 5.0f, 3.0f);
	_light0.dir = glm::vec3(0.0f, 0.0f, 0.0f);
	_light0.k1 = 0.1;
	_light0.intensity = 1.7;
	_light0.index = 0;
	_light0.error_count = 10; // to prevent error messages
	_light0.apply(_model->getProgram());

	_light1.pos = glm::vec3(0.0f, 5.0f, -3.0f);
	_light1.dir = glm::vec3(0.0f, 0.0f, 0.0f);
	_light1.k1 = 0.1;
	_light1.intensity = 1.0;
	_light1.index = 1;
	_light1.error_count = 10; // to prevent error messages
	_light1.apply(_model->getProgram());

	// set model to be rendered
	// This model is used to extract the point cloud.
	_pview->setModelFromFile(_temp_model_path);

	return true;
}


void SurfExtractApp::render_fcn(glm::mat4 proj_matrix, glm::mat4 view_matrix)
{

	// this runs the automatic point cloud sampler .
	if (!_pview_complete && _pview_start) {
		_pview_complete = _pview->draw_sequence();
		_pca->addData(_pview->getCurrData());

		_gl_pc->updatePoints();
	}
	else {
		// this saves the object once all has been done. 
		if (!_output_done && _output_path.size() > 0) {
			_pca->writeToFile(_output_path, "obj");
			_pca->writeToFile(_output_path, "ply");
			rewrite_obj_gfx();
			_output_done = true;
			_enable_render_normals = true;
		}
	}
	

	// render the 3D model. 
	if (_model && _enable_render_3d)
		_model->draw(proj_matrix, view_matrix);

	// render the points
	if(_gl_pc && _enable_render_points)
		_gl_pc->draw(proj_matrix, view_matrix);

	// render the normal vectors
	if (_gl_normals  && _enable_render_normals)
		_gl_normals->draw(proj_matrix, view_matrix);

}

/*
Set the prerenderer camera distanct
@param distance - the distance as float
*/
void SurfExtractApp::setCameraDistance(float distance)
{
	if (distance < 0.1) return ;

	_pview_camera_distance = distance;
	_pview_camera_distance_user_value = true;

	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0, _pview_camera_distance), glm::vec3(0.0f, 0.0f, 00.f), glm::vec3(0.0f, 1.0f, 0.0f));
	if (_renderer)
		_renderer->setViewMatrix(viewMatrix);
}

/*
Set the camera distance automatically. 
Note that this function requires a model loaded. 
Also, it wil not work anymore as soon as the function setCameraDistance was called by a user. 
*/
void SurfExtractApp::autoSetCameraDistance(void)
{
	if(_pview_camera_distance_user_value) return;

	// retrive the bounding box data
	float bbedge = _geometry_check.getMaxBBEdge();

	// set the camera distance to 1.75 or 1.4 x the max object extesnion
	float camera_multiplier = 1.75f;
	if(bbedge > 100.0) camera_multiplier = 1.4f;
	_pview_camera_distance = bbedge * camera_multiplier;

	// do nothing if the object is too close. 
	if (_pview_camera_distance < 0.1) return ;

	// set the values. 
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0, _pview_camera_distance), glm::vec3(0.0f, 0.0f, 00.f), glm::vec3(0.0f, 1.0f, 0.0f));
	if (_renderer){
		_renderer->setViewMatrix(viewMatrix);

		cout << "[INFO] - Set the camera distance to an auto value of c = " << _pview_camera_distance << endl;

		glm::mat4 projMatrix = glm::perspective(1.2f, (float)_window_width / (float)_window_height, 0.1f, _pview_camera_distance * 1.5f);
		_renderer->setProjectionMatrix(projMatrix);
	}
}



/*
Start the point cloud generation
*/
bool SurfExtractApp::start(void)
{
	_pview->create(_pview_camera_distance, _pview_sub);
	_pview_start = true;

	show3DModel(false);
	_renderer->start();

	return _pview_start;
}


/*
Return whether the renderer is done 
drawing all images for the surface sequence. 
@return false - if not done, otherwise true
*/
bool SurfExtractApp::done(void)
{
	return _pview_complete;
}


/*
Set the density of the point cloud
@param density - the point cloud density as voxel cell distance.
Must be >= 0.005
*/
void SurfExtractApp::setPointCloudDensity(float density)
{
	if (!_pca){
		if (_verbose) {
			cout << "[CRITICAL ERROR] - _pca not ready (setPointCloudDensity)." << endl;
		}
		return;
	}

	_pca->setPointCloudDensity(density);
	
}

/*
Resample an existing point cloud
@param density - the point cloud density as voxel cell distance.
Must be >= 0.005
*/
void SurfExtractApp::resample(float density)
{
	if (!_pca)return;

	_pca->resample(density);
	_gl_pc->updatePoints();

	
	cout << "[INFO] - Sampled " << _pca->getNumPoints() << " points." << endl;
	
}


/*
Save the point cloud to an  obj file.
@param path - string containing the path and file. 
Ending must be obj.
*/
bool SurfExtractApp::saveToObj(string path)
{
	if (!_pca)return false;

	check_output_path(path);

	_output_path = path;
	return _pca->writeToFile(path, "obj");
}

bool SurfExtractApp::setVerbose(bool verbose){
	if (!_pca)return false;

	_verbose = verbose;
	_pca->setVerbose(verbose);
	return true;
}


/*
Save the point cloud to an  obj file once the procedure is complete
@param path - string containing the path and file. 
Ending must be obj.
*/
void SurfExtractApp::setOutputFilename(string path)
{
	check_output_path(path);
	_output_path = path;
}


/*
Set the a scale parameter for the final point cloud.
The output point cloud will be scaled by this parameter
before written into a file.
@param scale - float value larger than 0.001;
*/
bool SurfExtractApp::setOutputScale(float scale)
{
	if (!_pca) return false;
		
	_pca->setOutputScale(scale);
	return true;
}


/*
Keyboard callback for the renderer
*/
void SurfExtractApp::keyboard_cb(int key, int action)
{
	//cout << key << " : " << action << endl;

	switch (action) {
	case GLFW_RELEASE:

		switch (key) {
		case 44:  // ,
			_pview_camera_distance -= 0.1;
			setCameraDistance(_pview_camera_distance);
			break;
		case 46: // .
			_pview_camera_distance += 0.1;
			setCameraDistance(_pview_camera_distance);
			break;
		case 49: // 1
			if (_enable_render_3d)
				this->show3DModel(false);
			else this->show3DModel(true);
			break;
		case 50: // 2
			if (_enable_render_points)
				this->showPointCloud(false);
			else this->showPointCloud(true);				
			break;
		case 82: //r
			cout << "[Input] - Enter density (" << _pca->getMinimumPointCloudDensity() << " < x < " << _geometry_check.getMaxBBEdge()  << "= no change), current: " << _pca->getPointCloudDensity() << " (press enter) \n";
			float density;
			std::cin >> density;
			resample(density);
			break;
		case 83: // s
			_pca->writeToFile(_output_path, "obj");
			_pca->writeToFile(_output_path, "ply");
			break;
		case 51: // 3
			if (_show_normals) _show_normals = false;
			else _show_normals = true;
			if (_gl_normals) _gl_normals->enable(_show_normals);
			break;

		}

		break;

	case GLFW_PRESS:
		break;
	}


}

void SurfExtractApp::rewrite_obj_gfx(void) 
{
	if (!_pca) return ;
		
	string path  = "";
	int idx0 = _output_path.find_last_of(".");
	path = _output_path.substr(0,idx0);
	path.append("_gfx.obj");

	_geometry_check.writeObj(path, _pca->getOutputScale());
}


/*
The function checks the output path and 
creates it if it does not exist. 
*/
void SurfExtractApp::check_output_path(std::string path)
{
	int idx0 = path.find_last_of("/");
	if (idx0 == -1) {
		idx0 =  path.find_last_of("\\");
	}

	// no path given, we save in the workpath 
	if (idx0 == -1) return;

	std::string sub_path = path.substr(0,idx0);

	if(!surfe::FileUtils::Exists(sub_path))
	{
		surfe::FileUtils::CreateDirectories(sub_path);
	}

}