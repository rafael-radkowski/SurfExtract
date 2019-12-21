#include "SurfacePosRenderer.h"


/*
Constructor
@param window_width, window_height - width and height of the current render window in pixel
@param render_width, render_height - width and height of the image to render in pixel
*/
surfe::SurfacePosRenderer::SurfacePosRenderer(int window_width, int window_height, int render_width, int render_height):
	_window_width(window_width), _window_height(window_height), _image_width(render_width), _image_height(render_height)
{
	_verbose = false;
	_model = NULL;
	_sphere = NULL;


	_light0.pos = glm::vec3(0.0f, 5.0f, 3.0f);
	_light0.dir = glm::vec3(0.0f, 0.0f, 0.0f);
	_light0.k1 = 0.1;
	_light0.intensity = 1.7;
	_light0.index = 0;
	
	_writer = new ImageWriter();
	_output_file_id = 0;
}


surfe::SurfacePosRenderer::~SurfacePosRenderer()
{
	if (_model)delete _model;
	if (_sphere) delete _sphere;
}

/*
Load a 3D model from a file
@param path_and_file - string containing the relative or absolute path.
@return true - if successfully loaded
*/
bool surfe::SurfacePosRenderer::setModelFromFile(std::string path_and_file)
{

	_program = cs557::LoadAndCreateShaderProgram("position_renderer.vs", "position_renderer.fs");

	_model = new cs557::OBJModel();
	_model->create(path_and_file, _program);
	_model->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	_light0.apply(_model->getProgram());

	_proj_matrix = glm::perspective(1.2f, (float)1024 / (float)1024, 0.1f, 1000.f);
	_view_matrix = glm::lookAt(glm::vec3(1.5f, 1.5f, 0.0f), glm::vec3(0.0f, 0.0f, 00.f), glm::vec3(0.0f, 1.0f, 0.0f));

	setViewMatrix(_view_matrix);

	prepareFBO();
	prepareHelpers();

	return true;
}

	
/*
Draw the object
*/
bool surfe::SurfacePosRenderer::draw(void)
{
	drawFBO();
	renderHelpers();

	return true;
}


/*
Set verbose mode to get more text output
@param v - true to enable verbose mode, false to disable it
*/
void surfe::SurfacePosRenderer::setVerbose(bool v)
{
	_verbose = v;
}


/*
Set the view matrix - the prerender image is rendered from this position
@param vm
*/
void surfe::SurfacePosRenderer::setViewMatrix(glm::mat4 vm)
{
	_view_matrix = vm;

	// update the light position so that it stays a camera front light. 
	glm::mat4 inv  = glm::inverse(_view_matrix);
	_light0.pos = glm::vec3(inv[3][0], inv[3][1], inv[3][2]);
	_light0.apply(_model->getProgram());
}


/*
Prepare a set of visual helpers
*/
bool surfe::SurfacePosRenderer::prepareHelpers(void)
{
	// sphere

	unsigned int program = cs557::LoadAndCreateShaderProgram("lit_scene.vs", "lit_scene.fs");
	_sphere = new cs557::Sphere();
	_sphere->create(0.05, 30, 40, program);

	_sphere_mat.diffuse_mat = glm::vec3(1.0, 0.0, 0.0);
	_sphere_mat.diffuse_int = 1.0;
	_sphere_mat.ambient_mat = glm::vec3(1.0, 0.0, 0.0);
	_sphere_mat.ambient_int = 0.2;
	_sphere_mat.specular_int = 0.5;
	_sphere_mat.specular_mat = glm::vec3(1.0, 1.0, 1.0);
	_sphere_mat.specular_s = 2.0;
	_sphere_mat.error_count = 10;
	_sphere_mat.apply(_sphere->getProgram());

	_light0.apply(_sphere->getProgram());

	//display
	// Load the shader program
	int shader = cs557::LoadAndCreateShaderProgram("display.vs", "display.fs");

	// create a plane
	_display.create(0.5, 0.5, shader);
	_display_m = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	_display_p = glm::perspective(1.57f, (float)400 / (float)400, 0.01f, 10.f);
	_display_v = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glUseProgram(shader); 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _color_texture_idx);
	int texture_location = glGetUniformLocation(shader, "tex");
	glUniform1i(texture_location, 0);
	glUniform1f(glGetUniformLocation(shader, "display_scale"), 2.0f);

	glUseProgram(0);

	return true;
}

/*
Render all helper content
*/
bool surfe::SurfacePosRenderer::renderHelpers(void)
{
	if (_sphere) {
		glm::mat4 inv  = glm::inverse(_view_matrix);
		_sphere->draw(_proj_matrix, _view_matrix, glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 0.0f)));
	}


	glUseProgram(_display.getProgram());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _color_texture_idx);
	_display.draw(_proj_matrix, _view_matrix, _display_m);

	return true;
}

/*
Prepare the fbo renderer
*/
bool surfe::SurfacePosRenderer::prepareFBO(void)
{
	// This function is part of RenderToTexture.h
	CreateRenderToTexture32Bit(_image_width, _image_height, _fboHidden, _color_texture_idx, _depth_texture_idx);
	// Reset to the regular buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// data for the image to fetch
	_data_rgb = (unsigned char*)malloc(_image_width * _image_height * 3 * sizeof(float));

	return true;
}

/*
Draw the FBO renderer
*/
bool surfe::SurfacePosRenderer::drawFBO(void)
{
	if (!_model)return false;

	glBindFramebuffer(GL_FRAMEBUFFER, _fboHidden);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set up our green background color
	static GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 0.0f };//{0.6f, 0.7f, 1.0f, 1.0f};
	static GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// Clear the entire buffer with our green color.
	glClearBufferfv(GL_COLOR, 0, clear_color);
	glClearBufferfv(GL_DEPTH, 0, clear_depth);

	// set the viewport. It must match the texture size.
	glViewport(0, 0,  _image_width, _image_height);

	_model->draw(_proj_matrix, _view_matrix);
	
	//-------------------------------------------------------------------------------------
	// get the data back 

	// read back the pixels
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glReadPixels(0, 0, _image_width, _image_height, GL_BGR, GL_FLOAT, _data_rgb);

	//glReadBuffer(GL_DEPTH_ATTACHMENT);
	//glReadPixels(0, 0, _image_width, _image_height, GL_DEPTH_COMPONENT, GL_FLOAT, _data_depth);


	cv::Mat image(_image_height, _image_width, CV_32FC3, _data_rgb);
	cv::Mat dst, output_rgb;
	cv::flip(image, dst, 0);

	// rgb image
	if (_verbose) {
		cv::imshow("Pos", dst);
		cv::waitKey(1);
	}


	if (_writer_enabled && _writer){
		_writer->write_32FC3(_output_file_id, dst, glm::inverse(_view_matrix));
		_output_file_id++;
	}

	//std::vector<Eigen::Vector3f> points;
	//m2p.process(dst, &points);

	//zxcout << "generated " << points.size() << " points" << endl;

	//-------------------------------------------------------------------------------------
	// reset render target abd viewport

	// switch back to the regular output buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// set the viewport to window size
	glViewport(0, 0, _window_width, _window_height);

	return true;
}



/*
Disable and enable the file writer
*/
bool surfe::SurfacePosRenderer::enable_writer(bool enable)
{
	_writer_enabled = enable;

	return true;
}

/*
Set an output path for the images
@param path - relative or absolute output path
@param name - name template for the output images.
*/
void surfe::SurfacePosRenderer::setOutputPath(std::string path, std::string name)
{
	_output_file_path = path;
	_output_file_name = name;

	if(_writer)
		_writer->setPathAndImageName(path, name);
}


/*
Return the latest data image.
@return OpenCV matrix of type CV_32F_C3 with the rendered positions
*/
cv::Mat surfe::SurfacePosRenderer::getCurrData(void)
{
	cv::Mat image(_image_height, _image_width, CV_32FC3, _data_rgb);
	cv::Mat dst, output_rgb;
	cv::flip(image, dst, 0);
	return dst;
}