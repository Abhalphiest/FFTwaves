#pragma once

//File: ocean
//Author: Margaret Dorsey
//
// The ocean files provide a class for our FFT ocean.
// Based on code by Keith Lantz.

#include"vector.h"
#include"vector.h"
#include"structs.h"
#include"shader.h"
#include"random.h"
#include"FFT.h"

#include "glm-0.9.2.6/glm/glm.hpp"
#include "glm-0.9.2.6/glm/gtc/matrix_transform.hpp"
#include "glm-0.9.2.6/glm/gtc/type_ptr.hpp"

//#include <windows.h>
#include "SOIL/SOIL.h"
#include <GL/glew.h>
#include <GL/glut.h>

//#define M_PI 3.14159265359

class ocean {
private:
	bool geometry;				// flag to render geometry or surface

	float g;				// gravity constant
	int N, Nplus1;				// dimension -- N should be a power of 2
	float A;				// phillips spectrum parameter -- affects heights of waves
	vector2 w;				// wind parameter
	float length;				// length parameter
	complex *h_tilde,			// for fast fourier transform
		*h_tilde_slopex, *h_tilde_slopez,
		*h_tilde_dx, *h_tilde_dz;
	FFT *fft;				// fast fourier transform

	vertex_ocean *vertices;			// vertices for vertex buffer object
	unsigned int *indices, *w_indices;			// indices for vertex buffer object
	unsigned int indices_count, w_indices_count;		// number of indices to render
	GLuint vbo_vertices, vbo_indices, vbo_windices;	// vertex buffer objects
	GLuint waterTexture, textureLoc; //texture stuff
	GLuint glProgram, glShaderV, glShaderF;	// shaders
	GLint vertex, normal, texture, light_position, projection, view, model;	// attributes and uniforms

protected:
	
public:
	//sets up everything we need, including GL resources, etc
	ocean(const int N, const float A, const vector2 w, const float length, bool geometry);
	//releases CPU memory
	~ocean();
	//releases GPU memory
	void release();
	void toggleGeometry() { geometry = !geometry; } //toggling shaded or wireframe rendering
	float dispersion(int n_prime, int m_prime);		// deep water dispersion factor
	float phillips(int n_prime, int m_prime);		// phillips spectrum calculation
	complex hTilde_0(int n_prime, int m_prime);		//complex conjugate height function
	complex hTilde(float t, int n_prime, int m_prime); //index based height function
	complex_vector_normal h_D_and_n(vector2 x, float t); //displacement vector and normal vector computation
	void evaluateWavesFFT(float t); //wrapper function for FFT computation and wave updates
	//render logic for ocean
	void render(float t, glm::vec3 light_pos, glm::mat4 Projection, glm::mat4 View, glm::mat4 Model);
};
