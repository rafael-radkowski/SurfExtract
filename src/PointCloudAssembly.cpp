#include "PointCloudAssembly.h"


PointCloudAssembly::PointCloudAssembly() {

	_verbose = false;

	_default_param.grid_x = 0.005;
	_default_param.grid_y = 0.005;
	_default_param.grid_z = 0.005;

	_user_param.grid_x = 0.005;
	_user_param.grid_y = 0.005;
	_user_param.grid_z = 0.005;
    _default_method = UNIFORM;

	_output_scale = 1.0;

	_m2p.setStride(1);
}

PointCloudAssembly::~PointCloudAssembly()
{

}

/*
Set the minimum acceptable distance between points,
defined via the sampling grid size
@param density -  a float values > 0 for the denisty.
*/
void PointCloudAssembly::setPointCloudDensity(float density)
{
	if (density < 0.005) {
		cout << "[ERROR] - The point cloud density is currently limited to 0.005 in function setPointCloudDensity." << endl;
		cout << "[ERROR] - Change the code or scale your model. However, the change will affect the runtime and memory requirements." << endl;
		return;
	}

	_user_param.grid_x = density;
	_user_param.grid_y = density;
	_user_param.grid_z = density;
}


/*
Set the a scale parameter for the final point cloud.
The output point cloud will be scaled by this parameter
before written into a file.
@param scale - float value larger than 0.0001;
*/
bool PointCloudAssembly::setOutputScale(float scale)
{
	if(scale < 0.0001){
		cout << "[Error] - Scale parameter " << scale << "is to small. The minimum applicable value is 0.0001." << endl;
		return false;
	}

	_output_scale = scale;
	return true;
}

/*
Add a data matrix to the current point cloud and invoke processing
The matrix needs to be of type CV_32F_C3 with width x height x 3, 
the channels encode the x, y, z values. 
All empty points must contain values = 0
@param data - the opencv matrix with the surface data as coordinates in 3D space. 
*/
void PointCloudAssembly::addData(cv::Mat data)
{
	// get the points  as vector from the matrix. 
	_points_processing.clear();
	_normals_processing.clear();
	_m2p.process(data, &_points_processing, &_normals_processing);

	if(_verbose)
		cout << "[INFO] - Generated " << _points_processing.size() << " points" << endl;


	// Concatenate points
	_points_original.points.insert( _points_original.points.end(), _points_processing.begin(), _points_processing.end() );
	_points_original.normals.insert( _points_original.normals.end(), _normals_processing.begin(), _normals_processing.end() );


	// Sample
	SamplingPointCloud::SetMethod(_default_method, _default_param);
	SamplingPointCloud::Sample(_points_original, _points_ready);


	// Sample with user values
	SamplingPointCloud::SetMethod(_default_method, _user_param);
	SamplingPointCloud::Sample(_points_ready, _points_final);

	if(_verbose)
		cout << "[INFO] - Sampled " << _points_final.points.size() << " points" << endl;
}


/*
Resample the point cloud with a new density value.
Note that a initial point cloud  must already exist
@param density - a float values > 0.005 for the denisty.
*/
void  PointCloudAssembly::resample(float density)
{
	if (_points_ready.size() == 0) {
		cout << "[WARNING] - Cannot resample a point cloud without generating one first" << endl;
	}

	setPointCloudDensity(density);

	// Sample with user values
	SamplingPointCloud::SetMethod(_default_method, _user_param);
	SamplingPointCloud::Sample(_points_ready, _points_final);

	if(_verbose)
		cout << "[INFO] - Re-sampled " << _points_final.points.size() << " points" << endl;
}

/*
Return the final point cloud
*/
PointCloud&	PointCloudAssembly::getPointCloud(void)
{
	return _points_final;
}


/*
Write the point cloud to an obj file
@param path_and_filename - string with the relative or abosolute path. 
*/
bool PointCloudAssembly::writeToFileOBJ(string path_and_filename)
{
	return LoaderObj::Write(path_and_filename, &_points_final.points, &_points_final.normals, _output_scale);
}

/*
Return the number of points
*/
int PointCloudAssembly::getNumPoints(void)
{
	return _points_final.points.size();
}