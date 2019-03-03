#ifndef __SAMPLING__
#define __SAMPLING__


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
