#include"shader.h"

// File: shader
// Author: Margaret Dorsey
//
// The shader files handle the loading, compiling,
// and program creation for shaders.
// Nothing too revolutionary.
//

//loads the shader text file
// Based on code by Warren Carithers from
// a previous project
char* loadFile(const char *filename) 
{
	FILE *fp;
	GLchar *content = NULL;
	int count = 0;

	if (filename != NULL) 
	{
		// Attempt to open the file
		// On Windows systems, may want to use "rt" here
		fp = fopen(filename, "r");
		if (fp != NULL) 
		{
			// Determine its length
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0) 
			{
			// Allocate the string buffer
			content = new GLchar[sizeof(GLchar) * (count + 1)];
			// Read the file into the buffer
			count = fread(content, sizeof(GLchar), count, fp);
			// Add a NUL terminator
			content[count] = '\0';
			}

			fclose(fp);
		}
		else 
		{
			perror(filename);
		}
	}
	else 
	{
		fprintf(stderr, "error:  no file name specified\n");
	}

	return content;

}

//creates a shader program GPU side
void createProgram(GLuint& glProgram, GLuint& glShaderV, GLuint& glShaderF, const char* vertex_shader, const char* fragment_shader) {
	glShaderV = glCreateShader(GL_VERTEX_SHADER);
	glShaderF = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* vShaderSource = loadFile(vertex_shader);
	const GLchar* fShaderSource = loadFile(fragment_shader);
	glShaderSource(glShaderV, 1, &vShaderSource, NULL);
	glShaderSource(glShaderF, 1, &fShaderSource, NULL);
	delete[] vShaderSource;
	delete[] fShaderSource;
	glCompileShader(glShaderV);
	glCompileShader(glShaderF);
	glProgram = glCreateProgram();
	glAttachShader(glProgram, glShaderV);
	glAttachShader(glProgram, glShaderF);
	glLinkProgram(glProgram);
	glUseProgram(glProgram);

	int  vlength, flength;
	char vlog[2048], flog[2048], plog[2048];
	glGetShaderInfoLog(glShaderV, 2048, &vlength, vlog);
	glGetShaderInfoLog(glShaderF, 2048, &flength, flog);
	glGetProgramInfoLog(glProgram, 2048, &flength, plog);
	std::cout << vlog << std::endl << std::endl << flog << std::endl << std::endl << plog << std::endl << std::endl;
}

//releases the shader program GPU side
void releaseProgram(GLuint& glProgram, GLuint glShaderV, GLuint glShaderF) {
	glDetachShader(glProgram, glShaderF);
	glDetachShader(glProgram, glShaderV);
	glDeleteShader(glShaderF);
	glDeleteShader(glShaderV);
	glDeleteProgram(glProgram);
}