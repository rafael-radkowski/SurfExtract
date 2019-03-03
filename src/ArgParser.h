#ifndef __ARGPARSER__
#define __ARGPARSER__

// stl
#include <iostream>
#include <string>
#include <vector>

// local
#include "types.h"

using namespace std;

namespace arlab
{

typedef struct _Arguments
{
	string  current_path;
	string	model_path_and_file;
	string	output_path_filename;

	int		windows_width;
	int		window_height;
	int		image_width;
	int		image_height;
	
	float	camera_distance;
	float	voxel_grid_size;


	bool	valid;
	bool	verbose;

	_Arguments()
	{

		current_path = "";
		model_path_and_file = "";
		output_path_filename = "output";

		windows_width = 1280;
		window_height = 1024;
		image_width = 1024;
		image_height = 1024;

		camera_distance = 1.3;
		voxel_grid_size = 0.04;

		verbose = false;
		valid = false;
	}


}Arguments;


class ArgParser
{
public:

	/*
	Parse the arguments
	@param argc - number of arguments
	@param argv - the argument line
	@return struct with arguments
	*/
	static Arguments Parse(int& argc, char** argv);

	/*
	Display help
	*/
	static void Help(void);

	/*
	Display all arguments
	*/
	static void Display(void);

private:

	/*
	Extract the current path
	*/
	static void Path(char* path);

	/*
	Extract the current path
	*/
	static void ParamError(string option);
};



}// namespace arlab


#endif