#pragma once
// File: skybox
// Author: Margaret Dorsey
//
// The skybox files handle the initialization and rendering
// of the cube map that acts as our skybox.
// A cube map essentially takes 6 images that seamlessly fit together (hopefully)
// and places them at infinite depth in each direction,
// giving the impression of detailed objects in the distance
// without actually rendering them.
//


//#include <windows.h> //REMOVE FOR UNIX PORT
#include "SOIL/SOIL.h" // SOIL library for image loading
#include <GL/glew.h>
#include <GL/glut.h>
#include <string> //can't honestly remember what I used this for
				//but it probably had a reason



#include "glm-0.9.2.6/glm/glm.hpp"
#include "glm-0.9.2.6/glm/gtc/matrix_transform.hpp"
#include "glm-0.9.2.6/glm/gtc/type_ptr.hpp"
#include "shader.h"
class skybox
{
	//GL locations of all our data
	GLuint texture;
	GLuint skyboxVAO, skyboxVBO;
	GLuint skyboxProgram, skyboxVS, skyboxFS;
	//hard coded file paths for our 6 faces
	// the order MATTERS
	char* faces[6] = {"skybox/nightsky_right.jpg","skybox/nightsky_left.jpg","skybox/nightsky_top.jpg","skybox/nightsky_bottom.jpg",
		"skybox/nightsky_back.jpg", "skybox/nightsky_front.jpg" };
public:
	//gets all the resources together that we need to render
	void setupSkybox();
	//rendering logic for skybox
	// changes some openGL states
	//reverts before returning
	void renderSkybox(glm::mat4 View, glm::mat4 Projection);
	//release the memory for all our resources
	void releaseSkybox();

};
