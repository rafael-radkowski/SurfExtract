#ifndef __SAMPLING__
#define __SAMPLING__
/*
class Sampling

The class provides functions to change the density of a point cloud by uniformly sample all provided points. 

Currently, only uniform sampling is supported, method Uniform.
The function calculates the max. expansion of the point cloud and allocates a uniform voxel grid, each voxel 
has the user-specified edge length grid_x, which can be set via the sampling parameters. 

- To assign a point cloud point to a voxel, each point's position is divided by the grid size.
This results in an integer, which indicates the voxel id. The point id - its index in the point vector - is stored in the voxel. 
- Note that all voxels are allocated with a value -1. Point ids are only stored in a voxel as long as the id is -1.
  Once a voxel is occupied, no new point will be stored. First comes first serves. Thus, no duplicates
  neither point coordinates nor normal vectors are merged.
- Points with invalid normal vectors (length != 1.0) are ignored.

Rafael Radkowski
Iowa State University
rafael@iastate.edu
March 2019
MIT License
------------------------------------------------------------------------------------------------------
Last edits:

Dec 14, 2019, RR:
- Added a warning that pops up if the number of voxels exceeds 1000. To may voxel need too much memory. S
*/

//stl
#include <iostream>
#include <string>
#include <limits>
#include <algorithm>

// Eigen
#include <Eigen/Dense>

// local
#include "Types.h"
#include "SamplingTypes.h"

using namespace std;

class SamplingPointCloud
{

public:

    /*
    Set the sampling method along with the sampling parameters
    @param method - the sampling method. Can be RAW, UNIFORM, and RANDOM
    @param param - sampling parameters of type SamplingParam. The parameter must belong to
    the set SamplingMethod.
    */
    static void SetMethod(SamplingMethod method, SamplingParam param);

    /*
    Start the sampling procedure
    @param src - location of the the source point cloud.
    @param dst - location of the the destination  point cloud.
    */
    static void Sample(PointCloud& src, PointCloud& dst, bool verbose = false);

private:
    /*
    Sample the point cloud with a uniform sampling filter. 
    This filter is voxel based and will put one point into a vocel 
    @param src - location of the the source point cloud.
    @param dst - location of the the destination  point cloud.
    @param param - the sampling parameters
    */
    static void Uniform( PointCloud& src, PointCloud& dst, SamplingParam param);


};

#endif
