# SurfExtract
The application extracts a point cloud from all visible surfaces of a mesh model; it ignores all hidden surfaces. 
The tool renders the surface of a 3D model from multiple locations around a sphere into a 32bit fbo. 
The pixels contain just model positions and a point for each valid pixel is created. 

![Figure 1: Overview working principle: The tool renders surface data from multiple locations into a 32 bit fbo. The data is sampled to generated point clouds. The point clouds are merged and a uniform voxel grid prevents duplicates.](https://github.com/rafael-radkowski/SurfExtract/blob/master/data/media/Overview.jpeg | width=300)
Figure 1: Overview working principle: The tool renders surface data from multiple locations into a 32 bit fbo. The data is sampled to generated point clouds. The point clouds are merged and a uniform voxel grid prevents duplicates.

Subsequent sampling prevent duplicates and also allows one to reduce the point cloud density via a voxel grid. 

![](https://github.com/rafael-radkowski/SurfExtract/blob/master/data/media/PointCloudSampling.jpeg | width=300)


### Prerequisites
All code runs (tested) on Windows 10, Version 1803.
It requires the following 3rd party tools:
 * [OpenCV v3.4.5](https://opencv.org)
 * [Eigen3 v3.3.7](http://eigen.tuxfamily.org)
 * [GLEW v2.1.0](http://glew.sourceforge.net)
 * [GLFW v3.2.1](https://www.glfw.org)
 * [glm, v0.9.9.3](https://glm.g-truc.net/0.9.9/index.html)
 
Additinally, all code from my OpenGL utils folder is required:
 * [OpenGL Helpers](https://github.com/rafael-radkowski/GLSupport)
