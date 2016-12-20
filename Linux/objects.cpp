#include "objects.h"

//
// File: objects
// Author: Margaret Dorsey
//
// The objects files handle building the pier and teapot for the requirements of 
// the CSCI 510 project. It is a very basic pile of code that neatly sidesteps the
// need for legitimate architecture/organization in the code base,
// and most values are hard coded and global.
//

//OpenGL variables

//horrible coding practice, but it does the job for this little toy demo
//shaders
GLuint objectProgram, objectVS, objectFS;
//dock and teapot
GLuint dockvao, dockvbo, dockibo, docknumIndices, dockTexture;
glm::mat4 dM; //dock model matrix

GLuint teapotvao, teapotvbo, teapotibo, teapotnumIndices, teapotTexture;
glm::mat4 tM; //teapot model matrix


//build objects essentially sets everything up that is needed
//to render our two objects
void buildObjects()
{
	//load meshes into GPU buffers
	setupMesh("dock.obj", dockvao, dockvbo, dockibo, docknumIndices);
	setupMesh("teapot.obj", teapotvao, teapotvbo, teapotibo, teapotnumIndices);
	//initialize our shader program for objects
	createProgram(objectProgram, objectVS, objectFS, "objects.vert", "objects.frag");
	//hard code our transformation matrices to place the objects in the world
	// (Don't do this at home, kids!)
	dM = glm::mat4(1.0);
	dM = glm::translate(dM, glm::vec3(-10, 29.5, 0));
	tM = glm::mat4(1.0);
	tM = glm::translate(tM, glm::vec3(-10, 29.5, 4.25));
	tM = glm::scale(tM, glm::vec3(.02f));

	//set up textures for both objects
	//they are set up to repeat when the UV coordinate leaves the
	// [0,1]x[0,1] plane
	glActiveTexture(GL_TEXTURE0);
	dockTexture = SOIL_load_OGL_texture
	(
		"woodtexture.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y 
	);
	glBindTexture(GL_TEXTURE_2D, dockTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glActiveTexture(GL_TEXTURE0);
	teapotTexture = SOIL_load_OGL_texture
	(
		"uvtexture.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y //no need to bother with mip mapping, etc
							//our textures can look as ugly as we want them to
	);
	glBindTexture(GL_TEXTURE_2D, teapotTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

//the rendering logic for our two objects
void drawObjects(glm::mat4 View, glm::mat4 Projection, glm::vec3 light_pos)
{
	//pass in the location of the light for lighting computation
	GLuint light_position = glGetUniformLocation(objectProgram, "light_position");
	GLuint textureLoc = glGetUniformLocation(objectProgram, "Texture");
	glUniform3f(light_position, light_pos.x, light_pos.y, light_pos.z);

	//dock goes first
	glUseProgram(objectProgram);
	//pass in the transform matrix for the dock, as well as
	//the matrices for the camera transforms
	GLuint m = glGetUniformLocation(objectProgram, "Model");
	glUniformMatrix4fv(m, 1, GL_FALSE, glm::value_ptr(dM));
	GLuint v = glGetUniformLocation(objectProgram, "View");
	glUniformMatrix4fv(v, 1, GL_FALSE, glm::value_ptr(View));
	GLuint p = glGetUniformLocation(objectProgram, "Projection");
	glUniformMatrix4fv(p, 1, GL_FALSE, glm::value_ptr(Projection));

	//pass in texture info
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, dockTexture);
	glUniform1i(textureLoc, 0);
	//pass in vertex info
	glBindVertexArray(dockvao);
	glBindBuffer(GL_ARRAY_BUFFER, dockvbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dockibo);
	//the layout of our interleaved data, to help the shader parse a whole
	//lot of floats
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);                       //position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));       //normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 6));     //UV
	//actually draw the thing
	glDrawElements(GL_TRIANGLES, docknumIndices, GL_UNSIGNED_INT, 0);


	//now the teapot
	glUseProgram(objectProgram);
	m = glGetUniformLocation(objectProgram, "Model"); //view and projection are the same
	glUniformMatrix4fv(m, 1, GL_FALSE, glm::value_ptr(tM));
	glBindVertexArray(teapotvao);
	glBindBuffer(GL_ARRAY_BUFFER, teapotvbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, teapotibo);

	//bind the second texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, teapotTexture);
	glUniform1i(textureLoc, 0);
	//the connection between attributes, interleaved data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);                       
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));      
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 6)); 
	glDrawElements(GL_TRIANGLES, teapotnumIndices, GL_UNSIGNED_INT, 0);
}

void releaseObjects()
{
	//textures
	glDeleteTextures(1, &dockTexture);
	glDeleteTextures(1, &teapotTexture);
	//shader program
	releaseProgram(objectProgram,objectVS,objectFS);
	//buffers
	glDeleteBuffers(1, &dockvbo);
	glDeleteBuffers(1, &dockvao);
	glDeleteBuffers(1, &dockibo);
	glDeleteBuffers(1, &teapotvbo);
	glDeleteBuffers(1, &teapotvao);
	glDeleteBuffers(1, &teapotibo);
}