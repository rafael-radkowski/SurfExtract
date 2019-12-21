#include "GeometryCheckup.h"


GeometryCheckup::GeometryCheckup()
{
	_verbose = true;
	_centroid[0] = 0;
	_centroid[1] = 0;
	_centroid[2] = 0;

	reset();
}


GeometryCheckup::~GeometryCheckup()
{
	reset();
}


/*
Center the obj model to its centroid and calculate the bouding box parameters
@param input_path_and_file -  string containg the path and the file. MUST BE AN OBJ file
@param output_path_and_file - string containg the output path and the file. MUST BE AN OBJ file
*/
bool GeometryCheckup::processObj(std::string input_path_and_file, std::string output_path_and_file)
{
	bool ret = loadObj(input_path_and_file);
	if(!ret) return false;

	Eigen::Vector3f centroid, boundingbox_size;

	// calculate the centroid
	ret = calculateGeometricCentroid(centroid);
	if(!ret) return false;

	// calculate the bouding box
	ret =  calculateBoundingBox( boundingbox_size);
	if(!ret) return false;

	// move the object to its centroid
	ret = setToBoundingBoxCenter();
	if(!ret) return false;

	// write the object to a file. 
	ret =  writeObj(output_path_and_file);
	if(!ret) return false;

	return ret;
}

/*!
Load an obj file. 
@param path_and_file - string containg the path and the file. 
@return true if the object was successfully loaded
*/
bool GeometryCheckup::loadObj(std::string path_and_file)
{
	if (!surfe::FileUtils::Exists(path_and_file)) {
		cout << "[ERROR] - GeometryCheckup: File " << path_and_file << " does not exist." << endl;
		return false;
	}

	// reset all data
	reset();

	_path_and_file = path_and_file;

	std::ifstream infile(path_and_file);

    if(!infile.is_open()){
        #ifdef __WIN32
        _cprintf("[ERROR] - GeometryCheckup: could not open file %s.\n", path_and_file.c_str())
        #else
         cout << "[ERROR] - GeometryCheckup: could not open file " <<  path_and_file << "." << endl;
        #endif
        return false;
    }


	_normals.reserve(2000);
	_points.reserve(2000);
	_original_file_content.reserve(4000);

	_first_line_data = -1;
	_mtl_line = -1;
	int count=0;

	string str;
    while(std::getline(infile, str))
    {
		
		std::vector<string> out =  GeometryCheckup::split(str, ' ');

		if(out.size() == 0) continue;

		if( out[0].compare("vn") == 0  ) {

			if(is_number(out[1]) && is_number(out[2]) && is_number(out[3])){
                float dnx=std::stof(out[1]);
                float dny=std::stof(out[2]);
                float dnz=std::stof(out[3]);
                _normals.push_back(Eigen::Vector3f((float)dnx,(float)dny,(float)dnz));
			}

			if(_first_line_data == -1)_first_line_data = count;
		}
		else if( out[0].compare("v") == 0) {
			if(is_number(out[1]) && is_number(out[2]) && is_number(out[3])){
                float dnx=std::stof(out[1]);
                float dny=std::stof(out[2]);
                float dnz=std::stof(out[3]);
                _points.push_back(Eigen::Vector3f((float)dnx,(float)dny,(float)dnz));
			}

			if(_first_line_data == -1)_first_line_data = count;
		}
		else
		{
			_original_file_content.push_back(str);
		}

		// store the used materials
		if( out[0].compare("usemtl") == 0  ) {
			_usemtl_list.push_back( out[1]);
		}else if(out[0].compare("mtllib") == 0){
			_mtllib_file = out[1];
			_mtl_line = count;
		}
		
		count++;
    }

	
	return true;
}


/*
Write the obj data to a file.
@param path_and_file - string containg the path and the file. 
*/
bool GeometryCheckup::writeObj(std::string path_and_file, float scale)
{
	// write the temporary material file
	processMtlFile(_path_and_file, path_and_file);



	std::ofstream outfile(path_and_file, std::ofstream::out);

    if(!outfile.is_open()){
        #ifdef __WIN32
        _cprintf("[LoaderObj] - Error: could not open file %s for writing.\n", file.c_str())
        #else
         cout << "[LoaderObj] - Error: could not open file " <<  path_and_file.c_str() << " for writing." << endl;
        #endif
        return false;
    }

	size_t N = _points.size();
	size_t M = _normals.size();

	//assert(_points.size() == _normals.size());

	outfile << "# Created by SurfExtract point cloud generation.\n";
	outfile << "# This model was created as part of the point cloud extraction process.\n";
	outfile << "# It is an intermediate file with a centered coordinate system.\n";
	outfile << "# Size: " << N << "\n\n";


	// write out the first line of original file data. 
	for(int i=0; i<_first_line_data; i++)
	{
		if(i == _mtl_line) outfile << "mtllib " << _new_mtllib_file << endl;
		else outfile << _original_file_content[i] << endl;
	}


	Eigen::Matrix3f T = Eigen::Matrix3f::Identity();
	T(0,0) = scale;
	T(1,1) = scale;
	T(2,2) = scale;
	Eigen::Matrix3f Tit = (T.inverse()).transpose(); 

	// write out the modified data
	for (int i = 0; i < N; i++) {
		Eigen::Vector3f p =  _points[i];
		Eigen::Vector3f o_p =  T * p;
		outfile << std::fixed << "v " << o_p[0] << " " << o_p[1] << " " << o_p[2] << "\n";
	}

	// write out the modified data
	for (int i = 0; i < M; i++) {
		Eigen::Vector3f n =  _normals[i];
		Eigen::Vector3f o_n =  Tit * n;
		o_n.normalize();
		outfile << std::fixed << "vn " << o_n[0] << " " << o_n[1] << " " << o_n[2] << "\n";
	}

	// write out the remaining original file data
	// write out the first line of original file data. 
	for(int i=_first_line_data; i<_original_file_content.size(); i++)
	{
		outfile << _original_file_content[i] << endl;
	}

	outfile.close();
	
}


//Helper functions
//static 
bool GeometryCheckup::is_number(std::string s)
{
    return !s.empty() && s.find_first_not_of("0123456789.-") == std::string::npos;
}



std::vector<std::string> GeometryCheckup::split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}


/*
Identify the centroid of the object. 
@param cx - location to store the x-coordinate of the centroid
@param cy - location to store the y-coordinate of the centroid
@param cz - location to store the z-coordinate of the centroid
*/
bool GeometryCheckup::calculateGeometricCentroid(Eigen::Vector3f& centroid)
{
	

	if (_points.size() == 0) {
		 if(_verbose){
			cout << "[ERROR] - GeometryCheckup: no points given for centroid calculation." << endl;
		 }
		return false;
	}

	int N = _points.size();
	float x = 0;
	float y = 0;
	float z = 0;

	for (auto p : _points) {
		x += p.x();
		y += p.y();
		z += p.z();
	}

	centroid.x() = x / N;
	centroid.y() = y / N;
	centroid.z() = z / N;

	_centroid[0] = centroid.x();
	_centroid[1] = centroid.y();
	_centroid[2] = centroid.z();

	 if(_verbose){
        cout << "[INFO] GeometryCheckup - geometric centroid x: " << centroid.x() << ". " << endl;
        cout << "[INFO] GeometryCheckup - geometric centroid y: " << centroid.y() << ". " << endl;
        cout << "[INFO] GeometryCheckup - geometric centroid z: " << centroid.z() << ". " << endl;
    }

	return true;
}

/*
Move the entire dataset to its centroid
@param cx - x-coordinate of the centroid
@param cy - y-coordinate of the centroid
@param cz - z-coordinate of the centroid
*/
bool GeometryCheckup::setToBoundingBoxCenter(void)
{
	if (_points.size() == 0 ) {
		if(_verbose){
			cout << "[ERROR] - GeometryCheckup: no points given to modify." << endl;
		 }
		return false;
	}

	if ( _bounding_box[0] == 0) {
		Eigen::Vector3f boundingbox_size;
		calculateBoundingBox(boundingbox_size);
	}

	
	float dist_x = (maxX - minX)/2.0 + minX;
	float dist_y = (maxY - minY)/2.0 + minY;
	float dist_z = (maxZ - minZ)/2.0 + minZ;


	size_t N = _points.size();
	for (int i = 0; i < N; i++) {
		_points[i].x() = _points[i].x() - dist_x;
		_points[i].y() = _points[i].y() - dist_y;
		_points[i].z() = _points[i].z() - dist_z;
	}

	return true;
}


bool GeometryCheckup::calculateBoundingBox(Eigen::Vector3f& boundingbox_size)
{
	if (_points.size() == 0) {
		if(_verbose){
			cout << "[ERROR] - GeometryCheckup: no points given for bounding box calculation." << endl;
		}
		return false;
	}

	int size = _points.size();
    //--------------
    // find min and max values 
    maxX =  std::numeric_limits<float>::min();
    maxY =  std::numeric_limits<float>::min();
    maxZ =  std::numeric_limits<float>::min();
    minX =  std::numeric_limits<float>::max();
    minY =  std::numeric_limits<float>::max();
    minZ =  std::numeric_limits<float>::max();

    for( auto p : _points){
       if(p.x() > maxX) maxX = p.x();
       if(p.x() < minX) minX = p.x();
       if(p.y() > maxY) maxY = p.y();
       if(p.y() < minY) minY = p.y();
       if(p.z() > maxZ) maxZ = p.z();
       if(p.z() < minZ) minZ = p.z();
    }

    if(_verbose){
        cout << "[INFO] GeometryCheckup - Min x: " << minX << ", max x: " << maxX << endl;
        cout << "[INFO] GeometryCheckup - Min y: " << minY << ", max y: " << maxY << endl;
        cout << "[INFO] GeometryCheckup - Min z: " << minZ << ", max z: " << maxZ << endl;
    }

	boundingbox_size.x()  = maxX - minX;
	boundingbox_size.y()  = maxY - minY;
	boundingbox_size.z()  = maxZ - minZ;

	_bounding_box[0] = boundingbox_size.x();
	_bounding_box[1] = boundingbox_size.y();
	_bounding_box[2] = boundingbox_size.z(); 

	cout << "[INFO] - Object dimensions" << endl;
	cout << "[INFO] - Dim x: " << _bounding_box[0] << "."  << endl;
    cout << "[INFO] - Dim y: " << _bounding_box[1] << "."  << endl;
    cout << "[INFO] - Dim z: " << _bounding_box[2] << "."  << endl;

	_max_size = _bounding_box[0];
	if(_bounding_box[1] >_max_size)
		_max_size = _bounding_box[1];
	if(_bounding_box[2] >_max_size)
		_max_size = _bounding_box[2];
	
	return true;
}


/*
Return the largest bounding box edge
*/
float GeometryCheckup::getMaxBBEdge(void)
{
	return _max_size;
}

/*
Enable or disable outputs
*/
bool GeometryCheckup::setVerbose(bool verbose)
{
	_verbose = verbose;

	return _verbose;
}


/*
Reset all internal data. 
*/
bool GeometryCheckup::reset(void)
{
	_points.clear();
	_normals.clear();
	_original_file_content.clear();
	_first_line_data = -1;
	_centroid[0] = 0;
	_centroid[1] = 0;
	_centroid[2] = 0;

	_bounding_box[0] = 0;
	_bounding_box[1] = 0;
	_bounding_box[2] = 0;


	_max_size = 0;
	_mtllib_file = "";
	_usemtl_list.clear();

	_new_mtllib_file = "";
	_path_and_file = "";
	_mtl_line = -1;


	maxX =  std::numeric_limits<float>::min();
    maxY =  std::numeric_limits<float>::min();
    maxZ =  std::numeric_limits<float>::min();
    minX =  std::numeric_limits<float>::max();
    minY =  std::numeric_limits<float>::max();
    minZ =  std::numeric_limits<float>::max();

	return true;
}



//check the material file and write a temporary material file out if non exists.
// @param path_and_file - the original material file
// @param output_path_and_file - the new material file, path and filename;
bool GeometryCheckup::processMtlFile(std::string path_and_file_3d_model, std::string output_path_and_file)
{

	///---------------
	// Extract the material path from the model path 
	string path = "";

	int idx = path_and_file_3d_model.find_last_of("/");
	if(idx == std::string::npos)
		idx = path_and_file_3d_model.find_last_of("\\");

	
	if(idx != std::string::npos){
		path = path_and_file_3d_model.substr(0,idx);
	}

	string mtl_file = path;
	mtl_file.append("/");
	mtl_file.append(_mtllib_file);



	///---------------
	// Extract the new material file name from the output file name. 

	std::string new_mtl_path = "";
	std::string new_mtllib_name = "";

	idx = output_path_and_file.find_last_of("/");
	if(idx == std::string::npos)
		idx = output_path_and_file.find_last_of("\\");

	
	if(idx != std::string::npos){
		new_mtl_path = output_path_and_file.substr(0,idx);
	}else
	{
		idx = 0;
	}

	int idx2 =  output_path_and_file.find_last_of(".");
	new_mtllib_name = output_path_and_file.substr(idx, idx2 - idx );
	new_mtllib_name.append(".mtl");

	if(new_mtllib_name[0] == '/') new_mtllib_name.erase(0,1);

	_new_mtllib_file = new_mtllib_name;

	if(idx > 0)
		new_mtl_path.append("/");
	new_mtl_path.append(new_mtllib_name);

	return writeMaterialFile(mtl_file, new_mtl_path);

}


//check the material file and write a temporary material file out if non exists.
bool GeometryCheckup::writeMaterialFile(std::string path_and_file, std::string output_path_and_file)
{
	bool mtl_file = surfe::FileUtils::Exists(path_and_file);

	std::ofstream outfile(output_path_and_file, std::ofstream::out);

	if(!outfile.is_open()){
		#ifdef __WIN32
		_cprintf("[LoaderObj] - Error: could not open file %s for writing.\n", file.c_str())
		#else
			cout << "[LoaderObj] - Error: could not open file " <<  output_path_and_file.c_str() << " for writing." << endl;
		#endif
		return false;
	}

	outfile << "# Created by SurfExtract point cloud generation.\n\n";

	if (mtl_file) { // material file exist

		std::ifstream infile(path_and_file);

		if(!infile.is_open()){
			#ifdef __WIN32
			_cprintf("[ERROR] - GeometryCheckup: could not open file %s.\n", path_and_file.c_str())
			#else
			 cout << "[ERROR] - GeometryCheckup: could not open file " <<  path_and_file << "." << endl;
			#endif
			return false;
		}

		string str;
		while(std::getline(infile, str))
		{
			outfile << str << endl;
		}

		infile.close();


	}else // no material file exists
	{

		for (int i = 0; i < _usemtl_list.size(); i++) {

			outfile << "newmtl " << _usemtl_list[i] << endl;
			outfile << "Ns 96.078431" << endl;
			outfile << "Ka 1.000000 1.000000 1.000000" << endl;
			outfile << "Kd 0.640000 0.261816 0.102188" << endl;
			outfile << "Ks 0.500000 0.500000 0.500000" << endl;
			outfile << "Ke 0.000000 0.000000 0.000000" << endl;
			outfile << "Ni 1.000000" << endl;
			outfile << "d 1.000000" << endl;
			outfile << "illum 2" << endl;
		}

		outfile << "\n" << endl;

		outfile << "newmtl default" << endl;
		outfile << "Ns 96.078431" << endl;
		outfile << "Ka 1.000000 1.000000 1.000000" << endl;
		outfile << "Kd 0.640000 0.261816 0.102188" << endl;
		outfile << "Ks 0.500000 0.500000 0.500000" << endl;
		outfile << "Ke 0.000000 0.000000 0.000000" << endl;
		outfile << "Ni 1.000000" << endl;
		outfile << "d 1.000000" << endl;
		outfile << "illum 2" << endl;
	
	}

	outfile.close();

	return true;
}


/*
The function computes the minimum recommended 
voxel density for any given object. 
*/
float GeometryCheckup::computeMinVoxelDensity(void)
{
	// we assume that we do not want more than 500 voxels along any axis. 
	const int max_voxel = 500;

	if(_max_size == 0) return 0.00;

	float density = (std::max)(_max_size / float(max_voxel), 0.005f);

	return density;
}