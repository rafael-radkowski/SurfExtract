#ifndef __GL_NORMAL_RENDERER__
#define __GL_NORMAL_RENDERER__
/*

Rafael Radkowski
Iowa State University
rafael@iastate.edu
March 2019
MIT License
------------------------------------------------------------------------------------------------------
Last edits:

Dec 15, 2019, RR
- Added a parameter to the function updateNormals to accomodate larger models. 
- Added a check that verifies that the number of points does not exceed the allocated vbo memory. 
- Added a function to automatically calculate the length of the normal vector, depending on the distance between
   camera and object. 
*/



// stl include
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// opencv
#include <opencv2/opencv.hpp>

// local
#include "VertexBuffers.h"			// create vertex buffer object
#include "ShaderProgram.h"			// create a shader program
#include "Types.h"

using namespace std;


class GLNormalsRenderer{

public:

        /*
		Create an point cloud model from a point cloud dataset
		@param pc - the point cloud dataset 
		*/
		GLNormalsRenderer(vector<Eigen::Vector3f>& dst_points, vector<Eigen::Vector3f>& dst_normals);



		/*
		Update the points using the existing references. 
		@param length - length of the normal vectors
		*/
		void updateNormals(float length = 1.0);


		/*
		Draw the obj model
		@param viewMatrix - a view matrix object
		@param projectionMatrix - a  4x4 projection matrix .
		*/
		void draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);


		/*
		Return the shader program
		@return - int containing the shader program
		*/
		int getProgram(void){return program;}


        /*
        Set the model matrix for this object.
        @param m - glm 4x4 matrix 
        */
        void setModelmatrix(glm::mat4 m){modelMatrix = m;}

		/*
		Enable the renderer. 
		@param value - true shows the normal vectors. 
		*/
		void enable(bool value);

private:

		/*
		Determine the length of the normal vectors as a function of
		the camera distance. The further the camera away, the larger 
		the renderer renders the normal vectors. 
		@param vm - a 4x4 view matrix. 
		*/
		float computeNormalLength(glm::mat4 vm);

		
		//---------------------------------------------------------


		int vaoID[1]; // Our Vertex Array Object
		int vboID[2]; // Our Vertex Buffer Object
		int iboID[1]; // Our Index  Object
		int program;
		int program_backup;

		int viewMatrixLocation;
		int modelMatrixLocation;
		int projMatrixLocation;


        std::vector<glm::vec3> points; 
	    std::vector<glm::vec3> normals;

		int _pos_location;
		int _norm_location;
		int _block;

		bool _enable;

        glm::mat4  modelMatrix; 
		glm::mat4	_projectionMatrix;
		glm::mat4	_viewMatrix;

	
		int _N; // number of vertices
		int _I; // number indices

		// reference to the original points and normals
		vector<Eigen::Vector3f>& _points;
		vector<Eigen::Vector3f>& _normals;


};



#endif