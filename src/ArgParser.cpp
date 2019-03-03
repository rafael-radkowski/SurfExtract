#include "ArgParser.h"


using namespace arlab;


namespace ArgParserTypes{

	Arguments opt;
	int error_count = 0;
}

using namespace ArgParserTypes;


/*
Parse the arguments
@param argc - number of arguments
@param argv - the argument line
@return struct with arguments
*/
Arguments ArgParser::Parse(int& argc, char** argv)
{
	//cout << argc << endl;
	//cout << argv[0] << endl;

	if (argc < 2) {
		Help();
		return opt;
	}

	opt.valid = true;

	// extract the path
	Path(argv[0]);

	// get the model
	opt.model_path_and_file = argv[1];

	int pos = 2;
	while(pos < argc)
	{
		string c_arg(argv[pos]); 

		if (c_arg.compare("-o") == 0) { // output path
			if (argc >= pos+1) opt.output_path_filename = string(argv[pos+1]);
			else ParamError(c_arg);
		}
		else if(c_arg.compare("-img_w") == 0){ // image width 
			if (argc >= pos) opt.image_width = atoi( string(argv[pos+1]).c_str() );
			else ParamError(c_arg);
		}
		else if(c_arg.compare("-img_h") == 0){ // image width 
			if (argc >= pos) opt.image_height =atoi(  string(argv[pos+1]).c_str() );
			else ParamError(c_arg);
		}
		else if(c_arg.compare("-wnd_w") == 0){ // window width 
			if (argc >= pos) opt.windows_width = atoi( string(argv[pos+1]).c_str() );
			else ParamError(c_arg);
		}
		else if(c_arg.compare("-wnd_h") == 0){ // window height  
			if (argc >= pos) opt.window_height =atoi(  string(argv[pos+1]).c_str() );
			else ParamError(c_arg);
		}
		else if(c_arg.compare("-c") == 0){ // camera distance
			if (argc >= pos) opt.camera_distance = atof(  string(argv[pos+1]).c_str() );
			else ParamError(c_arg);
		}
		else if(c_arg.compare("-d") == 0){ // voxel density
			if (argc >= pos) opt.voxel_grid_size = atof(  string(argv[pos+1]).c_str() );
			else ParamError(c_arg);
		}
		else if(c_arg.compare("-help") == 0 || c_arg.compare("-h") == 0){ // help
			Help();
		}
		else if(c_arg.compare("-verbose") == 0 ){ // help
			opt.verbose = true;
		}
	

		pos++;
	}


	if (opt.verbose)
		Display();

	return opt;
}


/*
Display help
*/
//static
void ArgParser::Help(void)
{
	cout << "[ERROR] - Missing parameters\nUsage:" << endl;
	cout << "SurfExtract [3d model path and filename] " << endl;
	cout << "Optional parameters:" << endl;
	cout << "\t-o [param] - set the output path" << endl;
	cout << "\t-c [param] \t- set the camera distance (float)." << endl;
	cout << "\t-d [param] \t- set the voxel grid size (float)." << endl;
	cout << "\t-help \t- displays this help menu" << endl;

	cout << "\nExample: SurfExtract ../data/modesl/engine_block.obj -o engine_block_pc.obj -c 4.5  -d=0.025\n" << endl;

	cout << "\nNot necessary but sometimes helpful parameters:" << endl;
	cout << "\t-img_w [param] \t- set the widht of the output image in pixels (integer)." << endl;
	cout << "\t-img_h [param] \t- set the height of the output image in pixels (integer)." << endl;
	cout << "\t-wnd_w [param] \t- set the widht of the application window in pixels (integer)." << endl;
	cout << "\t-wnd_h [param] \t- set the height of the application window in pixels (integer)." << endl;
}


/*
Display all arguments
*/
//static 
void ArgParser::Display(void)
{
	std::cout << "\nParameters:" << endl;
	std::cout << "Model:\t" << opt.model_path_and_file << endl;
	std::cout << "Output path:\t" << opt.output_path_filename << endl;
	std::cout << "Voxel grid density : " << opt.voxel_grid_size << endl;
	std::cout << "Camera distance radius: " << opt.camera_distance << endl;
	std::cout << "Image width:\t" << opt.image_width << endl;
	std::cout << "Image height:\t" << opt.image_height << endl;
	std::cout << "Wnd width:\t" << opt.windows_width << endl;
	std::cout << "Wnd height:\t" << opt.window_height << endl;


}


/*
Extract the current path
*/
//static 
void ArgParser::Path(char* path)
{
	string str(path);

	int idx = str.find_last_of("\\");
	if(idx == -1){ 
		idx = str.find_last_of("/");
	}
	opt.current_path = str.substr(0, idx+1);
}

/*
Extract the current path
*/
//static 
void ArgParser::ParamError(string option)
{
	cout << "[ERROR] - Parameter for option " << option << " is missing or invalid." << endl;
	error_count++;
}