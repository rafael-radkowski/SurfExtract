#include "LoaderOBJ.h"




//Helper functions
//static 
bool LoaderObj::is_number(std::string s)
{
    return !s.empty() && s.find_first_not_of("0123456789.-") == std::string::npos;
}


/*!
Load a point cloud .obj object from file
@param file = The file
@param loadedNormals = The output location of the loaded normals
@return cloud = The output of the loaded point cloud object
*/
//static 
bool LoaderObj::Read(string file, vector<Eigen::Vector3f >* dst_points, vector<Eigen::Vector3f >* dst_normals, bool invert_z)
{
    dst_points->clear();
    dst_normals->clear();

    std::ifstream infile(file);

    if(!infile.is_open()){
        #ifdef __WIN32
        _cprintf("[LoaderObj] - Error: could not open file %s.\n", file.c_str())
        #else
         cout << "[LoaderObj] - Error: could not open file " <<  file.c_str() << "." << endl;
        #endif
        return false;
    }

    int count=0;
    while(infile)
    {
        string str;
        infile >> str;
        if(str.compare("vn") == 0){
            count++;
            string nx, ny, nz;
            infile >> nx; infile >> ny; infile >> nz;
            // remove werid meshlab things and invalid data
            if(is_number(nx) && is_number(ny) && is_number(nz)){
                double dnx=std::stod(nx);
                double dny=std::stod(ny);
                double dnz=std::stod(nz);
                if (invert_z) dnz = -dnz;
                Eigen::Vector3f v((float)dnx,(float)dny,(float)dnz);
                v.normalize();
                dst_normals->push_back(v);
            }else{
                //Mark as invalid
                dst_normals->push_back(Eigen::Vector3f(0.0f,0.0f,0.0f));
            }
        }
        else if(str.compare("v") == 0){
            count++;
                double x, y, z;
                infile >> x; infile >> y; infile >> z;
                //_cprintf("v %d %f %f %f\n",count,x,y,z);
                if (invert_z) z = -z;
                Eigen::Vector3f p((float)x,(float)y,(float)z);
                dst_points->push_back(p);
        }
    }




    //Verifiy data////////////////////////
	if(dst_points->size()>0){
		//If the normals do not match the points clear the normals
		//to force them to be recalculated later.
		if(dst_points->size()!= dst_normals->size()){
            dst_normals->clear();
        }

		std::vector<Eigen::Vector3f>::iterator nitr = dst_normals->begin();
		std::vector<Eigen::Vector3f>::iterator pitr = dst_points->begin();

		//check for bad points
		while(nitr!=dst_normals->end() && pitr!=dst_points->end()){
			if((*pitr)[0]==0 && (*pitr)[1]==0 && (*pitr)[2]==0){
				//bad point delete
				pitr=dst_points->erase(pitr);
				if(dst_normals->size()>0){
					nitr=dst_normals->erase(nitr);
				}
			}
            else{
				nitr++;
				pitr++;
			}
		}
		nitr = dst_normals->begin();
		pitr = dst_points->begin();
		int ncount=0;
		//count the number of undefined normals
		while(nitr!=dst_normals->end()){
			if((*nitr).x()==0 && (*nitr).y()==0 && (*nitr).z()==0){
				ncount++;
			}
			nitr++;
		}

		//If more then 10% of the normals are undefined delete all of the normals
		// and recalculate them later
		if(ncount>dst_normals->size()*.1){
			dst_normals->clear();
			//MessageBox(0, "More then 10% of the normals are undefined!", "Error:", MB_OK);
		}
        else{
			//Remove the points with undefined normals
			nitr = dst_normals->begin();
			pitr = dst_points->begin();
			//check for bad normals

			int count_e = 0;
			while(nitr!=dst_normals->end() && pitr!=dst_points->end()){
				if((*nitr).x()==0 && (*nitr).y()==0 && (*nitr).z()==0){
					//bad normal delete

					pitr = dst_points->erase(pitr);
					nitr = dst_normals->erase(nitr);
					// RR, Dec. 16, 2015: that's better

				}else{
					nitr++;
					pitr++;
				}
				count_e++;
			}
		}

    
    }

    return true;

}



/*
Write the point cloud data to a file
@param file - string containing path and name
@param dst_points - vector of vector3f points containing x, y, z coordinates
@param dst_normals - vector of vector3f normal vectors index-aligned to the points.
@param scale_points - float value > 0.0 that scales all points and normal vectors. 
*/
//static
bool LoaderObj::Write(string file, vector<Eigen::Vector3f>* dst_points, vector<Eigen::Vector3f>* dst_normals, float scale_points )
{

	std::ofstream outfile(file, std::ofstream::out);

    if(!outfile.is_open()){
        #ifdef __WIN32
        _cprintf("[LoaderObj] - Error: could not open file %s for writing.\n", file.c_str())
        #else
         cout << "[LoaderObj] - Error: could not open file " <<  file.c_str() << " for writing." << endl;
        #endif
        return false;
    }

	size_t N = dst_points->size();

	assert(dst_points->size() == dst_normals->size());

	outfile << "# Created by SurfExtract point cloud generation.\n";
	outfile << "# Size: " << N << "\n\n";

	Eigen::Matrix3f T = Eigen::Matrix3f::Identity();
	T(0,0) = scale_points;
	T(1,1) = scale_points;
	T(2,2) = scale_points;

	// inverse transpose
	// not really required now. But we most likely rotate the object in future.
	// so it should already be inside.
	Eigen::Matrix3f Tit = (T.inverse()).transpose(); 

	int inv_cout = 0; // counts invalid normal vectors 
	for (int i = 0; i < N; i++) {

		Eigen::Vector3f o_p =  T *(*dst_points)[i];
		Eigen::Vector3f o_n =  Tit * (*dst_normals)[i];
		o_n.normalize();

	
		if(o_n.norm() == 0.0){
			inv_cout++;
		}

		outfile << std::fixed << "v " << o_p[0] << " " << o_p[1] << " " << o_p[2] << "\n";
		outfile << std::fixed << "vn " << o_n[0] << " " << o_n[1] << " " << o_n[2] << "\n";
	}

	cout << "[INFO] - Saves " << N << " points and normal vectors to " <<  file.c_str() << "." << endl;
	if(inv_cout > 0 ) cout << "[INFO] - " <<   inv_cout << " normal vector(s) invalid (0,0,0)" << endl;
	outfile.close();
}