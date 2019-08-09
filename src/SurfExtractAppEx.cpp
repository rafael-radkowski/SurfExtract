#include "SurfExtractAppEx.h"



SurfExtractAppEx::SurfExtractAppEx()
{
	_model_path_to_file = "";
}

SurfExtractAppEx::~SurfExtractAppEx()
{

}


/*
Start the opengl interface. 
*/
bool SurfExtractAppEx::startGUI(void)
{
	if(_renderer)
		return _renderer->start();
	return false;
}


/*
Load the 3D model from a file
*/
bool SurfExtractAppEx::loadModelEx(string path_to_file)
{
	// load the 3d model
	unsigned int program = cs557::LoadAndCreateShaderProgram("lit_scene.vs", "lit_scene.fs");
	_model = new cs557::OBJModel();
	_model->create(path_to_file, program);
	_model->setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

	_light0.pos = glm::vec3(0.0f, 5.0f, 3.0f);
	_light0.dir = glm::vec3(0.0f, 0.0f, 0.0f);
	_light0.k1 = 0.1;
	_light0.intensity = 1.7;
	_light0.index = 0;
	_light0.apply(_model->getProgram());

	_light1.pos = glm::vec3(0.0f, 5.0f, -3.0f);
	_light1.dir = glm::vec3(0.0f, 0.0f, 0.0f);
	_light1.k1 = 0.1;
	_light1.intensity = 1.0;
	_light1.index = 1;
	_light1.apply(_model->getProgram());

	_model_path_to_file = path_to_file;
	return true;
}
