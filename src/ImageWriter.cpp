#include "ImageWriter.h"

//#define LOAD_TEST

ImageWriter::ImageWriter()
{

	_output_file_path = "path";
	_output_file_name = "image";

	_logfile_name = "render_log.csv";


	// delete the log file if one exist 
	string list_str = "./";
	list_str.append(_output_file_path);
	list_str.append("/");
	list_str.append(_logfile_name);
#ifdef _USE_EXP
	if (std::experimental::filesystem::exists(list_str))
		std::experimental::filesystem::remove(list_str);
#else
	if (std::filesystem::exists(list_str))
		std::filesystem::remove(list_str);
#endif
	

}

ImageWriter::~ImageWriter()
{

}


/*
Set the output path and name
@param path - the output file path
@param name - the file name as string
*/
void ImageWriter::setPathAndImageName(string path, string name)
{
	_output_file_path = path;
	_output_file_name = name;

	checkFolder(_output_file_path);
}


/*
Write the image data to a file
@param index - the image id;
@param rgb - cv::Mat& with the rbg image content
@param depth - cv::Mat with the depth values
@para pose - the current camera pose
*/
bool ImageWriter::write(int index, cv::Mat& rgb, cv::Mat& normals, cv::Mat& depth, glm::mat4 pose)
{
	string name = _output_file_path;
	name.append("/");
	name.append(to_string(index));
	name.append("_");
	name.append(_output_file_name);

	string name_rgb = name;
	name_rgb.append("_rgb.png");

	string name_normals = name;
	name_normals.append("_normals.png");

	string name_depth = name;
	name_depth.append("_depth.png");

	string name_mat = name;
	name_mat.append("_pose.txt");

	// Delete all clear buffer values from the depth map.
	cv::Mat output_depth;
	//output_depth = depth.clone();
	float *input = (float*)(depth.data);
	for(int j = 0;j < depth.rows;j++){
		for(int i = 0;i < depth.cols;i++){
			float d = input[depth.cols * j + i ] ;
			if (d == 1.0) input[depth.cols * j + i] = 0.0;
		}
	}


	// png only permits 16 bit
	cv::Mat depth_16UC1, normals_16UC3;
	depth.convertTo(depth_16UC1, CV_16UC1, 65535 );
	normals.convertTo(normals_16UC3, CV_16UC3, 65535 );

	cv::imwrite(name_rgb, rgb);
	cv::imwrite(name_depth, depth_16UC1);
	cv::imwrite(name_normals, normals_16UC3);

#ifdef LOAD_TEST
	cv::imshow("16bit", normals_16UC3);
	cout << "Write " << name_normals << " as " << type2str(normals_16UC3.type()) << endl;
#endif
	
	Eigen::Matrix4f mat;
	Eigen::Quaternionf q;
	mat << pose[0][0], pose[1][0], pose[2][0], pose[3][0],
		pose[0][1], pose[1][1], pose[2][1], pose[3][1],
		pose[0][2], pose[1][2], pose[2][2], pose[3][2],
		pose[0][3], pose[1][3], pose[2][3], pose[3][3];


	MatrixFileUtils::WriteMatrix4f(name_mat, mat, "pose:");
	MatrixHelpers::MatrixToQuaternion(mat, q);


	string list_str = "./";
	list_str.append(_output_file_path);
	list_str.append("/");
	list_str.append(_logfile_name);

	
	std::ofstream of(list_str, std::ifstream::out | std::ifstream::app);

	if (of.is_open())
	{
		of << to_string(index) << "," << name_rgb << "," << name_normals << "," << name_depth << "," <<
			name_mat << "," <<  pose[3][0]<< "," <<  pose[3][1]  << "," <<  pose[3][2] <<
			"," <<  q.x() << "," <<  q.y() <<"," <<  q.z() <<"," <<  q.w() << "\n";
	}
	of.close();


#ifdef LOAD_TEST

	cv::Mat in_depth = cv::imread(name_depth, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_GRAYSCALE  );
	
	cv::Mat in_depth_norm;
	in_depth.convertTo(in_depth_norm, CV_32FC1, 1.0/65535.0);


	cv::Mat in_normal = cv::imread(name_normals, CV_LOAD_IMAGE_ANYDEPTH  | CV_LOAD_IMAGE_COLOR  );
	cv::Mat in_normal_norm;
	in_normal.convertTo(in_normal_norm, CV_32FC3, 1.0/65535.0);

	cout << "Read " << name_normals << " as " << type2str(in_normal.type()) << endl;
	//cv::normalize(in_depth, in_depth_norm, 0, 255, cv::NORM_MINMAX, CV_8UC3);
	cv::imshow("load test_normal", in_normal_norm);
	cv::imshow("load test_depth", in_depth_norm);
	cv::waitKey(1);

	/*
	string debug = name_depth;
	debug.append(".csv");


	ofstream of2(debug);

	for (int i = 0; i < in_depth.rows; i++)
	{
		for (int j = 0; j < in_depth.cols; j++)
		{
			float data = in_depth.at<float>(i, j);

			float uv = data;
			of2 << uv << ", ";
		}
		of2 << "\n";
	}
	of2.close();
	*/


#endif

//#define TXT_DEBUG
#ifdef TXT_DEBUG

	string debug = name_depth;
	debug.append(".csv");


	ofstream of2(debug);

	for (int i = 0; i < depth_16UC1.rows; i++)
	{
		for (int j = 0; j < depth_16UC1.cols; j++)
		{
			short data = depth_16UC1.at<short>(i, j);

			short uv = data;
			of2 << uv << ", ";
		}
		of2 << "\n";
	}
	of2.close();


	string debug2 = name_normals;
	debug2.append(".csv");

	

	ofstream of3(debug2);

	for (int i = 0; i < normals.rows; i++)
	{
		for (int j = 0; j < normals.cols; j++)
		{
			cv::Vec3f data = normals.at<cv::Vec3f>(i, j);

			float uv = (float)data[0];
			of3 << uv << ", ";
		}
		of3 << "\n";
	}
	of3.close();


#endif
	return true;
}



/*
Write the image data to a file
@param index - the image id;
@param image - cv::Mat of type CV_32F_C3 with normal vectors 
@para pose - the current camera pose
*/
bool ImageWriter::write_32FC3(int index, cv::Mat& image, glm::mat4 pose)
{
	string name = _output_file_path;
	name.append("/");
	name.append(to_string(index));
	name.append("_");
	name.append(_output_file_name);


	string name_normals = name;
	name_normals.append("_16FC3.png");

	string name_mat = name;
	name_mat.append("_pose.txt");


	// png only permits 16 bit
	cv::Mat  normals_16UC3;
	image.convertTo(normals_16UC3, CV_16UC3, 65535 );

	cv::imwrite(name_normals, normals_16UC3);

#ifdef LOAD_TEST
	cv::imshow("16bit", normals_16UC3);
	cout << "Write " << name_normals << " as " << type2str(normals_16UC3.type()) << endl;
#endif
	
	Eigen::Matrix4f mat;
	Eigen::Quaternionf q;
	mat << pose[0][0], pose[1][0], pose[2][0], pose[3][0],
		pose[0][1], pose[1][1], pose[2][1], pose[3][1],
		pose[0][2], pose[1][2], pose[2][2], pose[3][2],
		pose[0][3], pose[1][3], pose[2][3], pose[3][3];


	MatrixFileUtils::WriteMatrix4f(name_mat, mat, "pose:");
	MatrixHelpers::MatrixToQuaternion(mat, q);

	/*
	string list_str = "./";
	list_str.append(_output_file_path);
	list_str.append("/");
	list_str.append(_logfile_name);

	
	std::ofstream of(list_str, std::ifstream::out | std::ifstream::app);

	if (of.is_open())
	{
		of << to_string(index) << "," << name_rgb << "," << name_normals << "," << name_depth << "," <<
			name_mat << "," <<  pose[3][0]<< "," <<  pose[3][1]  << "," <<  pose[3][2] <<
			"," <<  q.x() << "," <<  q.y() <<"," <<  q.z() <<"," <<  q.w() << "\n";
	}
	of.close();
	*/

	return true;
}




/*
Check whether the path exists.
Create a folder if the path does not exist.
@param path - string with the relative or absolute path.
*/
bool ImageWriter::checkFolder(string path)
{
	bool ret = std::experimental::filesystem::exists(path);

	if (ret)
	{
		// delete the log file if one exist 
		string list_str = "./";
		list_str.append(_output_file_path);
		list_str.append("/");
		list_str.append(_logfile_name);
#ifdef _USE_EXP
		if (std::experimental::filesystem::exists(list_str))
			std::experimental::filesystem::remove(list_str);
#else
		if (std::filesystem::exists(list_str))
			std::filesystem::remove(list_str);
#endif
	}



	if (!ret) {

		std::experimental::filesystem::create_directory(path);
	}

	// create a file with a header if no file exist.
	string list_str = "./";
	list_str.append(_output_file_path);
	list_str.append("/");
	list_str.append(_logfile_name);

	// write a header if the file does not exist
	if (!FileUtils::Exists(list_str)) {
		// create a header
		std::ofstream of(list_str, std::ifstream::out | std::ifstream::app);
		if (of.is_open()){
			of << "index,rgb_file,normals_file,depth_file,mat_file,tx,ty,tz,qx,qy,qz,qw\n";
		}
		of.close();
	}

	return true;
}


//---------------------------------------------------------
// members

