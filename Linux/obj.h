#pragma once

// File:	object
// Author:	Margaret Dorsey
//
// The object files handle the loading and setup
// of .obj file meshes, with normals and UV coordinates.
// Also passes all the data neatly to GPU side.
//

#include<sstream> //for brainless file IO
#include<fstream>
#include<vector> //for REALLY brainless buffer construction

#define UINT unsigned int

#include "glm-0.9.2.6/glm/glm.hpp"
#include "glm-0.9.2.6/glm/gtc/matrix_transform.hpp"
#include "glm-0.9.2.6/glm/gtc/type_ptr.hpp"

//#include <windows.h> //REMOVE FOR UNIX PORT

#include <GL/glew.h>
#include <GL/glut.h>


//easy way to store our data per vertex
//to pass to GPU in one big interleaved buffer
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

//loads obj files, fills parameter vectors
void loadObj(const char* filename, std::vector<Vertex>& verts, std::vector<UINT>& indices);
//calls loadObj, then passes the vertex data to the GPU for us. Sets the location parameters 
//in lieu of a return value.
void setupMesh(const char* filename, GLuint &vao, GLuint &vbo, GLuint &ibo, GLuint &numIndices);
