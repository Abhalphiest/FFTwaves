#pragma once

// File: shader
// Author: Margaret Dorsey
//
// The shader files handle the loading, compiling,
// and program creation for shaders.
// Nothing too revolutionary.
//

#include <windows.h>// REMOVE FOR UNIX PORT

#include <GL/glew.h>
#include <GL/glut.h>

#include<iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <math.h>

#include "complex.h"

//loads the shader text file
char* loadFile(const char *filename);

//creates a shader program GPU side
void createProgram(GLuint& glProgram, GLuint& glShaderV, GLuint& glShaderF, const char* vertex_shader, const char* fragment_shader);
//releases the shader program GPU side
void releaseProgram(GLuint& glProgram, GLuint glShaderV, GLuint glShaderF);