#include "PolyhedronViewRenderer.h"


/*
Constructor
@param - window_width, window_height - width and height of the opengl, glfw output image in pixel
@param - image_width, image_height - width and height of the image to be rendered and saved to a file.
*/
surfe::PolyhedronViewRenderer::PolyhedronViewRenderer(int window_width, int window_height, int image_width, int image_height) :
	SurfacePosRenderer(window_width, window_height, image_width, image_height)
{
	_segments = 30;
	_rows = 45;
	_radius = 1.0;
	_center = glm::vec3(0.f,0.f,0.f);
	_up = glm::vec3(0.0f, 1.0f, 0.0f);
	_N = 0;
	_N_current = 0;
	_camera_distance = 1.3;

	_subdivisions = 0;
}

surfe::PolyhedronViewRenderer::~PolyhedronViewRenderer()
{

}



/*
Set the number of subdivisions for the polyheder
The polyheder points define the camera view points. 
@param subdivisions - number of subdivisions. 0 is an Icosahedron
*/
void surfe::PolyhedronViewRenderer::create(float camera_distance, int subdivisions)
{

	if(_verbose)
		cout << "[INFO] - Created Polyhedron."  << endl;

	if (subdivisions >= 0) {
		_subdivisions = subdivisions;
	}
	else {
		cout << "[ERROR] - The minimum number for subdivisions is 0." << endl;
		_subdivisions = 0;
	}

	if (camera_distance > 0.1){
		_camera_distance = camera_distance;
	}
	
	_points.clear();
	_normals.clear();


	std::pair< vector<glm::vec3>, vector<glm::ivec3> > final_mesh;
	final_mesh  = surfe::PolyhedronGeometry::Create(_subdivisions);

	_points = final_mesh.first;
	_normals = final_mesh.first;

	_N = _points.size();
	
	if(_verbose)
		cout << "[INFO] - Created " << _N << " coordinates around a sphere." << endl;

	return;
}


/*
Draw the image sequence and save all images to a file
@return - false, if images still need to be rendered.
			true, id all images were saved to a file.
*/
bool surfe::PolyhedronViewRenderer::draw_sequence(void)
{
	if (_N_current < _N) {
		glm::vec3 p = _points[_N_current];
		glm::vec3 n = _normals[_N_current];
		glm::vec3 eye = n * glm::vec3(_camera_distance);


		// This prevents that the view matrix becomes NaN when eye and up vector align. 
		float ang = glm::dot(glm::normalize(eye), _up);
		float s = 1.0;
		if (ang < 0) s = -1.0;
		if (abs(ang) > 0.999)
			_up = glm::vec3(0.0, 0.0, -s);
		else
			_up = glm::vec3(0.0, 1.0, 0.0);


		glm::mat4 vm = glm::lookAt(eye, _center, _up );
		setViewMatrix(vm);

		enable_writer(true);
		if(_verbose)
			cout << "[INFO] - Render image " << _N_current << " from pos: " << eye[0] << " : " << eye[1] << " : " << eye[2]<< endl;
		draw();
		_N_current++;
		
		if (_N_current == _N){
			if(_verbose)
				cout << "[INFO] - DONE - rendered " << _N_current << " sets." <<  endl;
		}
		return false;
	}
	else {
		glm::vec3 p = _points[0];
		glm::vec3 n = _normals[0];
		glm::vec3 eye = n * glm::vec3(_camera_distance);
		glm::mat4 vm = glm::lookAt(eye, _center, _up);
		setViewMatrix(vm);

		enable_writer(false);
		draw();
		return true;
	}
	
}





