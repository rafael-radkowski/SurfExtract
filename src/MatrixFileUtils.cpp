#include "MatrixFileUtils.h"






/*
Read a Eigen matrix from a file
*/
Eigen::Matrix4f MatrixFileUtils::ReadMatrix4f(string path_and_file)
{
	Eigen::Matrix4f	matrix = Eigen::Matrix4f::Identity();


	if (!std::experimental::filesystem::exists(path_and_file))
	{
		_cprintf("\n[MatrixFileUtils] - ERROR: file %s does not exist. Matrix is identity.\n", path_and_file.c_str());
		return matrix;
	}


	ifstream in(path_and_file);

	if (in.is_open())
	{
		string label;
		in >> label;
		int i = 0;
		while (in)
		{
			
			float value;
			

			if (i < 16) {
				in >> value;
				matrix(i / 4, i % 4) = value;
				i++;
			}
			else
			{
				in >> label;
			}
		}
		in.close();
	}

	return matrix;
}


/*
Write a matrix to a file
*/
//static 
void MatrixFileUtils::WriteMatrix4f(string path_and_file, Matrix4f matrix, string label)
{
	ofstream of(path_and_file.c_str(), std::ofstream::out);

	if (!of.is_open()) {
		_cprintf("\n[MatrixFileUtils] - ERROR: file %s could not be written.\n", path_and_file.c_str());
		return;
	}


	if (label.length() > 0)
		of << label << "\n";


	for (int i = 0; i < 16; i++) {
		if (i % 4 == 0 && i > 0)of << "\n";
		of << matrix(i / 4, i % 4) << "\t";
	}

	of.close();

}





/*
Read a Eigen matrix from a file
*/
Eigen::Matrix3f MatrixFileUtils::ReadMatrix3f(string path_and_file)
{
	Eigen::Matrix3f	matrix = Eigen::Matrix3f::Identity();


	if (!std::experimental::filesystem::exists(path_and_file))
	{
		_cprintf("\n[MatrixFileUtils] - ERROR: file %s does not exist. Matrix is identity.\n", path_and_file.c_str());
		return matrix;
	}


	ifstream in(path_and_file);

	if (in.is_open())
	{
		string label;
		in >> label;
		int i = 0;
		while (in)
		{

			float value;


			if (i < 9) {
				in >> value;
				matrix(i / 3, i % 3) = value;
				i++;
			}
			else
			{
				in >> label;
			}
		}
		in.close();
	}

	return matrix;
}


/*
Write a matrix to a file
*/
//static 
void MatrixFileUtils::WriteMatrix3f(string path_and_file, Matrix3f matrix, string label)
{
	ofstream of(path_and_file.c_str(), std::ofstream::out);

	if (!of.is_open()) {
		_cprintf("\n[MatrixFileUtils] - ERROR: file %s could not be written.\n", path_and_file.c_str());
		return;
	}


	if (label.length() > 0)
		of << label << "\n";


	for (int i = 0; i < 9; i++) {
		if (i % 3 == 0 && i > 0)of << "\n";
		of << matrix(i / 3, i % 3) << "\t";
	}

	of.close();

}



/*
Read a Eigen matrix from a file
*/
Eigen::VectorXf MatrixFileUtils::ReadVectorXf(string path_and_file)
{
	Eigen::VectorXf	matrix(5);


	if (!std::experimental::filesystem::exists(path_and_file))
	{
		_cprintf("\n[MatrixFileUtils] - ERROR: file %s does not exist. Matrix is identity.\n", path_and_file.c_str());
		return matrix;
	}


	ifstream in(path_and_file);

	int length = matrix.size();

	if (in.is_open())
	{
		string label;
		in >> label;
		int i = 0;
		while (in)
		{

			float value;


			if (i < length) {
				in >> value;
				matrix(i) = value;
				i++;
			}
			else
			{
				in >> label;
			}
		}
		in.close();
	}

	return matrix;
}


/*
Write a matrix to a file
*/
//static 
void MatrixFileUtils::WriteVectorXf(string path_and_file, VectorXf matrix, string label)
{
	ofstream of(path_and_file.c_str(), std::ofstream::out);

	int size = matrix.size();

	if (!of.is_open()) {
		_cprintf("\n[MatrixFileUtils] - ERROR: file %s could not be written.\n", path_and_file.c_str());
		return;
	}


	if (label.length() > 0)
		of << label << "\n";


	for (int i = 0; i < size; i++) {
		of << matrix(i) << "\t";
	}

	of.close();

}
