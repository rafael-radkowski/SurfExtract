#include "PolyhedronGeometry.h"


namespace PolyhedronGeometry_types
{
	vector<glm::vec3>		_vertices;
	vector<glm::ivec3>		_triangle_index;
	int						_I; // num indices
}


using namespace PolyhedronGeometry_types;
using namespace arlab;
/*
Create an Icosahedron or Polyhedron, depending on the num of subdivisions. 
@param divisions - the number of subdivisons. 0 creates an Icosahedron;
@return - two lists. the first one is the list with all points. 
			the second one is an index list with all triangles
			<vertices, triangle indices >
*/
//static 
std::pair< vector<glm::vec3>, vector<glm::ivec3> > PolyhedronGeometry::Create(int subdivisions)
{
	// create the basic Icosahedron
	CreateIcosahedron();

	// subdivide it. 
	for (int i=0; i<subdivisions; ++i)
	{
		_triangle_index = subdivide(_vertices, _triangle_index);
	}
 
	return make_pair(_vertices, _triangle_index);


}


/*
Create an Icosahedron
*/
void PolyhedronGeometry::CreateIcosahedron(void)
{
	_vertices.clear();
	_triangle_index.clear();


	const float X =.525731112119133606f;
	const float Z =.850650808352039932f;
	const float N = 0.f;

	//	{-X,N,Z}, {X,N,Z}, {-X,N,-Z}, {X,N,-Z},
 // {N,Z,X}, {N,Z,-X}, {N,-Z,X}, {N,-Z,-X},
//  {Z,X,N}, {-Z,X, N}, {Z,-X,N}, {-Z,-X, N}


	_vertices.push_back(glm::vec3(-X, N, Z));
	_vertices.push_back(glm::vec3(X,N,Z));
	_vertices.push_back(glm::vec3(-X,N,-Z));
	_vertices.push_back(glm::vec3(X,N,-Z));

	_vertices.push_back(glm::vec3(N,Z,X));
	_vertices.push_back(glm::vec3(N,Z,-X));
	_vertices.push_back(glm::vec3(N,-Z,X));
	_vertices.push_back(glm::vec3(N,-Z,-X));

	_vertices.push_back(glm::vec3(Z,X,N));
	_vertices.push_back(glm::vec3(-Z,X, N));
	_vertices.push_back(glm::vec3(Z,-X,N));
	_vertices.push_back(glm::vec3(-Z,-X, N));


//	{0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
 // {8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
 // {7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
 // {6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}

	_triangle_index.push_back(glm::vec3(0,4,1));
	_triangle_index.push_back(glm::vec3(0,9,4));
	_triangle_index.push_back(glm::vec3(9,5,4));
	_triangle_index.push_back(glm::vec3(4,5,8));
	_triangle_index.push_back(glm::vec3(4,8,1));

	_triangle_index.push_back(glm::vec3(8,10,1));
	_triangle_index.push_back(glm::vec3(8,3,10));
	_triangle_index.push_back(glm::vec3(5,3,8));
	_triangle_index.push_back(glm::vec3(5,2,3));
	_triangle_index.push_back(glm::vec3(2,7,3));

	_triangle_index.push_back(glm::vec3(7,10,3));
	_triangle_index.push_back(glm::vec3(7,6,10));
	_triangle_index.push_back(glm::vec3(7,11,6));
	_triangle_index.push_back(glm::vec3(11,0,6));
	_triangle_index.push_back(glm::vec3(0,1,6));

	_triangle_index.push_back(glm::vec3(6,1,10));
	_triangle_index.push_back(glm::vec3(9,0,11));
	_triangle_index.push_back(glm::vec3(9,11,2));
	_triangle_index.push_back(glm::vec3(9,2,5));
	_triangle_index.push_back(glm::vec3(7,2,11));
}




int PolyhedronGeometry::vertex_for_edge(Lookup& lookup, vector<glm::vec3>& vertices, int first, int second)
{
	Lookup::key_type key(first, second);
	if (key.first>key.second)
		std::swap(key.first, key.second);
 
	auto inserted=lookup.insert({key, vertices.size()});

	if (inserted.second)
	{
		glm::vec3& edge0 = vertices[first];
		glm::vec3& edge1=vertices[second];
		glm::vec3 point = glm::normalize(edge0+edge1);

		vertices.push_back(point);
	}
 
	return inserted.first->second;
}


/*
Subdivide the Icosahedron triangles
*/
vector<glm::ivec3> PolyhedronGeometry::subdivide(vector<glm::vec3>& vertices, vector<glm::ivec3> triangles)
{
	Lookup lookup;
	vector<glm::ivec3> result;
 
	for (auto each:triangles)
	{
		std::array<int, 3> mid;
		for (int edge=0; edge<3; edge++)
		{
			mid[edge]=vertex_for_edge(lookup, vertices, each[edge], each[(edge+1)%3]);
		}
 
		result.push_back({each[0], mid[0], mid[2]});
		result.push_back({each[1], mid[1], mid[0]});
		result.push_back({each[2], mid[2], mid[1]});
		result.push_back({mid[0], mid[1], mid[2]});
	}
 
  return result;
}
