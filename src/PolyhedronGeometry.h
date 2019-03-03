#pragma once
/*
Class PolyhedronGeometry

Create an Icosahedron and extend it to a Polyhedron by recursive decomposition.

Algorithm credits to MARIUS ELVERT,
see https://schneide.blog/2016/07/15/generating-an-icosphere-in-c/ for details. 

Rafael Radkowski
Iowa State University
rafael@iastate.edu
+1 (515) 294 7044
Jan 2019
All copyrights reserved
*/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <array>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>  // transformation
#include <glm/gtx/quaternion.hpp> // quaternions


using namespace std;

namespace arlab
{

	class PolyhedronGeometry {
	
		public:

			/*
				Create an Icosahedron or Polyhedron, depending on the num of subdivisions. 
				@param subdivisions - the number of subdivisons. 0 creates an Icosahedron;
				@return - two lists. the first one is the list with all points. 
						  the second one is an index list with all triangles
						  <vertices, triangle indices >
			*/
			static std::pair< vector<glm::vec3>, vector<glm::ivec3> > Create(int subdivisions);
	
		private:
			// lookup table 
			typedef  std::map<std::pair<int, int>, int> Lookup;
			
			/*
			Create an Icosahedron
			*/
			static void CreateIcosahedron(void);



			static int vertex_for_edge(Lookup& lookup, vector<glm::vec3>& vertices, int first, int second);

			/*
			Subdivide the Icosahedron triangles
			*/
			static  vector<glm::ivec3> subdivide(vector<glm::vec3>& vertices, vector<glm::ivec3> triangles);



	};


}//namespace arlab