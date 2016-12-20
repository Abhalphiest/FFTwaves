#pragma once
//
// File: objects
// Author: Margaret Dorsey
//
// The objects files handle building the pier and teapot for the requirements of 
// the CSCI 510 project. It is a very basic pile of code that neatly sidesteps the
// need for legitimate architecture/organization in the code base,
// and most values are hard coded and global.
//

#ifndef _OBJECTS_H
#define _OBJECTS_H
//REMOVE FOR UNIX PORT
#include <windows.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include "SOIL\SOIL.h"  //SOIL image library for image loading

#include "glm-0.9.2.6/glm/glm.hpp" //matrix math libraries
#include "glm-0.9.2.6/glm/gtc/matrix_transform.hpp"
#include "glm-0.9.2.6/glm/gtc/type_ptr.hpp"

#include "obj.h"
#include "shader.h"



//build objects essentially sets everything up that is needed
//to render our two objects
void buildObjects();
//rendering logic to be called from main
void drawObjects(glm::mat4 View, glm::mat4 Projection, glm::vec3 light_pos);
//releases the openGL resources we used
void releaseObjects();

#endif