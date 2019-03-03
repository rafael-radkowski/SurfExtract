#ifndef __POLYHEDRONVIEW_RENDERER__
#define __POLYHEDRONVIEW_RENDERER__
/*
class PolyhedronViewRenderer

This class renders images by selecting the viewpoints from the vertices of an polyhedron. 
The points of a polyhedron are uniform distributed. Thus, the delta between each camera is equal. 
The renderer draws the rgb image, normal map, and depth map (linear) into a fbo and writes them into a file.

Set the number of subdivisions to generate a polyhedron (function create). 
Note that 0 subdivisions generates an Icosahedron.

Features:
- Renders the 3d model as RGB image and writes it into a file CV_8UC3)
- Renders the normal map and writes it into a file (CV_32FC1, float).
- Renders the depth map (linear) and writes the values into a file  (CV_32FC1, float).

Usage:

poly_renderer = new PolyhedronViewRenderer(1280, 1024, 1280, 1024);
poly_renderer->setVerbose(true); // set first to get all the output info
poly_renderer->setModel("path to model");
poly_renderer->setOutputPath("output");
poly_renderer->create(1.5,2);

Loop over 
poly_renderer->draw_sequence();
until the function returns 'true'. 'false' means that the sequence is incomplete. 


Rafael Radkowski
Iowa State University
rafael@iastate.edu
+1 (515) 294 7044
Jan 2019
All copyrights reserved

------------------------------------------------------------------
last edited:
Jan 21, 2019:
- Updated the up * eye  vector check to prevent that the view matrix becomes NaN. 
	Verified that the model is correctly oriented. 

March 4, 2019
- Re-purposed. Took this class from the DNN helper project and replaced the base class
	with SurfacePosRenderer
*/


// stl
#include <iostream>
#include <vector>
#include <string>

// opencv
#include <opencv2/opencv.hpp>


// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>  // transformation
#include <glm/gtx/quaternion.hpp> // quaternions

#include "PolyhedronGeometry.h" // for the Polyhedron geometry
#include "SurfacePosRenderer.h"

class PolyhedronViewRenderer : public SurfacePosRenderer
{
public:

	/*
	Constructor
	@param - window_width, window_height - width and height of the opengl, glfw output image in pixel
	@param - image_width, image_height - width and height of the image to be rendered and saved to a file.
	*/
	PolyhedronViewRenderer(int window_width = 1280, int window_height = 1024, int image_width = 2560, int image_height = 2048);
	~PolyhedronViewRenderer();

	/*
	Set the number of subdivisions for the polyheder
	The polyheder points define the camera view points. 
	@param subdivisions - number of subdivisions. 0 is an Icosahedron
	*/
	void create( float camera_distance,  int subdivisions = 4);

	/*
	Draw the image sequence and save all images to a file
	@return - false, if images still need to be rendered. 
			  true, id all images were saved to a file. 
	*/
	bool draw_sequence(void);


private:




	//--------------------------------------------------------------
	// members

	int					_segments;
	int					_rows;
	double				_radius;
	glm::vec3			_center;
	glm::vec3			_up;

	float				_camera_distance; 

	int					_N; // max number of points.
	int					_N_current; // current point to render

	std::vector<glm::vec3> _points;
	std::vector<glm::vec3> _normals;

	int						_subdivisions;

};




#endif