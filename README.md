# SurfExtract
The application extracts a point cloud from all visible surfaces of a mesh model; it ignores all hidden surfaces. 
The tool renders the surface of a 3D model from multiple locations around a sphere into a 32bit fbo. 
The pixels contain just model positions and a point for each valid pixel is created. 

![Figure 1: Overview working principle: The tool renders surface data from multiple locations into a 32 bit fbo. The data is sampled to generated point clouds. The point clouds are merged and a uniform voxel grid prevents duplicates.](https://github.com/rafael-radkowski/SurfExtract/blob/master/data/media/Overview.jpeg)
Figure 1: Overview working principle: The tool renders surface data from multiple locations into a 32 bit fbo. The data is sampled to generated point clouds. The point clouds are merged and a uniform voxel grid prevents duplicates.

Subsequent sampling prevent duplicates and also allows one to reduce the point cloud density via a voxel grid. 

![](https://github.com/rafael-radkowski/SurfExtract/blob/master/data/media/PointCloudSampling.jpeg )

### Prerequisites
All code runs (tested) on Windows 10, Version 1803. It was developed with Visual Studio 2017.
It requires the following 3rd party tools:
 * [OpenCV v3.4.5](https://opencv.org)
 * [Eigen3 v3.3.7](http://eigen.tuxfamily.org)
 * [GLEW v2.1.0](http://glew.sourceforge.net)
 * [GLFW v3.2.1](https://www.glfw.org)
 * [glm, v0.9.9.3](https://glm.g-truc.net/0.9.9/index.html)
 
Additinally, all code from my OpenGL utils folder is required:
 * [OpenGL Helpers](https://github.com/rafael-radkowski/GLSupport)

### Usage
Use command line arguments to configure the application; syntax:
```
SurfExtract.exe [input_path_and_file.obj] -o [output_path_and_file.obj] -c [camera_distance] -d [voxel_grid_distance]
```
with
* -o the output path and file. Currently, only .obj files are written.
* -c the camera distance. Find a distance that covers your model size. 
* -d uniform voxel grid size. This values determines the number of points. 

Example:
```
 SurfExtract.exe ../data/models/Rubber_Duck_02.obj -o point_cloud.obj  -c 4.5 -d 0.025
```

### Known issues and shortcommings
* The .obj model input requires an mtl material file. Otherwise, the obj loader does not process the file. Create an empty one if you do not have one.
* Mind your model size. The default camera is quite close. So if your model vertex values are huge, you might end up inside the model. This will be changed in future. 
