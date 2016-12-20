// 
// File: main.cpp
// Author: Margaret Dorsey
//
// The main file sets up GLUT windowing and callbacks, and starts the runtime loop.
//

#include "glm-0.9.2.6/glm/glm.hpp"
#include "glm-0.9.2.6/glm/gtc/matrix_transform.hpp"
#include "glm-0.9.2.6/glm/gtc/type_ptr.hpp"


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <vector>
#include "structs.h"
#include "ocean.h"
#include "skybox.h"
#include "objects.h"


//REMOVE FOR UNIX SYSTEMS
//#include <windows.h>

#include <GL/glew.h>
#include <GL/glut.h>

// dimensions of the drawing window, hardcoded for the purposes of this project
const int w_width = 600;
const int w_height = 600;

//global for simplicity for now, this would normally be handled
//more elegantly in a more sophisticated/complete architecture
ocean* myOcean;
skybox mySkybox;
float rotx=0.0f, roty=15.0f; //for camera rotation

//initialize GLUT, set up callbacks, etc
bool init(int argc, char** argv);
//our keyboard input function
void keyboard(unsigned char c, int i, int j);
//our update function, we do drawing here as well
//to avoid fixed frame rate from display func
void update();



int main(int argc, char**argv)
{
	//initialize GLUT and GLEW
	bool success = init(argc, argv);
	if (!success) //failed to set up a windowed environment
	{
		exit(1);
	}

	//build the ocean, the skybox, and the two objects
	myOcean = new ocean(64, 0.0003f, vector2(32.0f, 32.0f), 64, false);
	mySkybox.setupSkybox();
	buildObjects();
	glutMainLoop(); //enter our loop
	myOcean->release(); //give back our memory
	delete myOcean;
	releaseObjects();
	mySkybox.releaseSkybox(); //three different release methods for
								//three things... /sigh
	return 0;
}

bool init(int argc, char** argv)
{
	//initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	//make window
	glutInitWindowSize(w_width, w_height);
	glutCreateWindow("Margaret Dorsey Final Project");
	//initialize GLEW
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "GLEW initialization error: %s\n", glewGetErrorString(err));
		return false;
	}

	//set up callbacks
	glutKeyboardFunc(keyboard);
	glutIdleFunc(update);
	glEnable(GL_DEPTH_TEST);
	return true;
}

//this function is called automatically
//by GLUT in response to keyboard input
void keyboard(unsigned char c, int i, int j)
{
	//adjusts the rotation of the camera based
	//on keyboard input
	switch (c) {
	case 'w': //look up when w is held
		rotx += 1.0;
		if (rotx >= 10.0)
			rotx = 10.0;
		break;
	case 's': //look down with s
		rotx -= 1.0;
		if (rotx <= -30.0)
			rotx = -30.0;
		break;
	case 'd': //look right with d
		roty += 1.0;
		if (roty >= 60.0)
			roty = 60.0;
		break;
	case 'a': //look  left with a
		roty -= 1.0;
		if (roty <= 10.0)
			roty = 10.0;
		break;
	case 'g': //change geometry between wireframe and shaded
		myOcean->toggleGeometry();
		break;
	}
}
void update()
{
		//transformation matrices and location of the light
		//hard code for now, for simplicity
		static glm::mat4 Projection = glm::perspective(45.0f, (float)w_width / (float)w_height, 0.1f, 1000.0f);
		static glm::mat4 View;
		static glm::mat4 Model = glm::mat4(1.0f);
		static glm::vec3 light_position;

		//camera starting position is calculated every frame (we could do this
		//much, much, better /more efficiently)
		View = glm::mat4(1.0f);
		View = glm::rotate(View, rotx, glm::vec3(-1.0f, 0.0f, 0.0f));
		View = glm::rotate(View, roty, glm::vec3(0.0f, 1.0f, 0.0f));
		View = glm::translate(View, glm::vec3(10, -30, -5));
		light_position = glm::vec3(350.0f, 100.0f, -1000.0f);

		//get time since last frame
		float t1 = (float)(glutGet(GLUT_ELAPSED_TIME) / 1000.0);
		// rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		myOcean->render(t1, light_position, Projection, View, Model);
		drawObjects(View,Projection,light_position);
		mySkybox.renderSkybox(View, Projection); //draw skybox last
												 // swap the buffers
		glutSwapBuffers();						//so the frame we just drew is
												//now visible
	
	
}




