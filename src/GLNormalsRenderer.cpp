#include "GLNormalsRenderer.h"


#define MAX_POINTS 500000


namespace nsGLNormalsRenderer
{

	static const string vs_string_410 =
		"#version 410 core                                                 \n"
		"                                                                   \n"
		"uniform mat4 projectionMatrix;                                    \n"
		"uniform mat4 viewMatrix;                                           \n"
		"uniform mat4 modelMatrix;                                          \n"
		"in vec3 in_Position;                                               \n"
		"in vec3 in_Normal;                                                  \n"
		"out vec3 pass_Color;                                               \n"
		"out vec2 pass_Texture;												\n"
		"                                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
		"    pass_Color = in_Normal;                                         \n"
		"	gl_PointSize = 2.0;												\n"	
		"}                                                                 \n";

	// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
	static const string fs_string_410 =
		"#version 410 core                                                 \n"
		"                                                                  \n"
		"in vec3 pass_Color;                                                 \n"
		"in vec2 pass_Texture;												\n"
		"out vec4 color;                                                    \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    color = vec4(0.8,0.8,0.0, 0.2);                               \n"
		"}                                                                 \n";



};


using namespace nsGLNormalsRenderer;


 /*
Create an point cloud model from a point cloud dataset
@param pc - the point cloud dataset 
@param shader_program - overwrite the default shader program by passing a hander to the constructor
*/
GLNormalsRenderer::GLNormalsRenderer(vector<Eigen::Vector3f>& dst_points, vector<Eigen::Vector3f>& dst_normals):
	_points(dst_points), _normals(dst_normals)
{

	_enable = false;
	_block = false;
    program = -1;
	_N = 0;
	_I = 0;



	float center_x = 0.0;
	float center_y = 0.0;
	float center_z = 0.0;


	std::vector<int> indices;

	program = -1;
	// create a shader program only if the progrm was not overwritten. 
	if(program == -1)
		program = cs557::CreateShaderProgram(vs_string_410, fs_string_410);

    // Find the id's of the related variable name in your shader code. 
	projMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
	viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
	modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader
	_pos_location = glGetAttribLocation(program, "in_Position");
	_norm_location = glGetAttribLocation(program, "in_Normal");

    points.clear();
    normals.clear();

	int size = MAX_POINTS;

	// fill the vertex buffer object with garbage
	for(int i=0; i<size; i++)
	{
        points.push_back( glm::vec3(0.0,0.0, i * 0.01 ) );
        normals.push_back(glm::vec3(0.0,0.0, i * 0.01 ) );
	}
	_N = points.size();

	// create a vertex buffer object
	cs557::CreateVertexObjects33(vaoID, vboID, &points[0].x, &normals[0].x, _N, _pos_location, _norm_location );

    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
}


/*
Update the points using the existing references. 
*/
void GLNormalsRenderer::updateNormals(float length)
{
	_N = _normals.size();

	if (_N <= 0)return;
	if(_N  > MAX_POINTS) _N = MAX_POINTS;

	for (int i = 0; i < _N; i++) {
		points[i * 2] = glm::vec3(_points[i].x(), _points[i].y(), _points[i].z());
		points[i*2+1] =   glm::vec3(_points[i].x() + length * _normals[i].x(), _points[i].y() + length * _normals[i].y(), _points[i].z() + length * _normals[i].z());
	}


	_block = true;
	    // Enable the shader program
	glUseProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object
	glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)(2*_N *3* sizeof(GLfloat)),(void*)&points[0]); // Set the size and data of our VBO and set it to STATIC_DRAW

								  //Color
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Bind our second Vertex Buffer Object
	glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)(2*_N * 3* sizeof(GLfloat)), (void*) &points[0]); // Set the size and data of our VBO and set it to STATIC_DRAW

	glBindVertexArray(0); // Disable our Vertex Buffer Object

	_block = false;

	//draw(_projectionMatrix, _viewMatrix);

}


/*
Draw the obj model
@param viewMatrix - a view matrix object
@param modelMatrix - a model matrix object.
*/
void GLNormalsRenderer::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	if (_enable == false) return;
	if (_block) return;
	_projectionMatrix = projectionMatrix;
	_viewMatrix = viewMatrix;

	// update the normal vectors
	updateNormals(computeNormalLength(viewMatrix));

    // Enable the shader program
	glUseProgram(program);

	// this changes the camera location
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // send the view matrix to our shader
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // send the model matrix to our shader
	glUniformMatrix4fv(projMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // send the projection matrix to our shader

	// Bind the buffer and switch it to an active buffer
	glBindVertexArray(vaoID[0]);

	// Draw the triangles
 	glDrawArrays(GL_LINES, 0, 2*_N);
	
	// Unbind our Vertex Array Object
	glBindVertexArray(0);

	// Unbind the shader program
	glUseProgram(0);

}


/*
Enable the renderer. 
@param value - true shows the normal vectors. 
*/
void GLNormalsRenderer::enable(bool value)
{
	_enable = value;
}


/*
Determine the length of the normal vectors as a function of
the camera distance. The further the camera away, the larger 
the renderer renders the normal vectors. 
@param vm - a 4x4 view matrix. 
*/
float GLNormalsRenderer::computeNormalLength(glm::mat4 vm)
{
	float length = 0.1;

	float dist = (std::abs)(vm[3][2]);
	length = (std::max)( dist/10.0f, 0.01f);
	
	return length;
}