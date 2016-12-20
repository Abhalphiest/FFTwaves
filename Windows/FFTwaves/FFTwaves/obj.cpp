#include "obj.h"
#include <iostream>

// File:	object
// Author:	Margaret Dorsey
//
// The object files handle the loading and setup
// of .obj file meshes, with normals and UV coordinates.
// Also passes all the data neatly to GPU side.
//

using namespace std; //laziness

//loads obj files, fills parameter vectors
// Based on obj loader by Chris Cascioli from a previous project of 
// mine
void loadObj(const char* filename, std::vector<Vertex>& verts, std::vector<UINT>& indices)
{
	// File input object
	std::ifstream obj(filename);

	// Check for successful open
	if (!obj.is_open())
		return;

	// Variables used while reading the file
	std::vector<glm::vec3> positions;     // Positions from the file
	std::vector<glm::vec3> normals;       // Normals from the file
	std::vector<glm::vec2> uvs;           // UVs from the file         
	unsigned int vertCounter = 0;        // Count of vertices/indices
	char chars[100];                     // String for line reading

										 // Still good?
	while (obj.good())
	{
		// Get the line (100 characters should be more than enough)
		obj.getline(chars, 100);

		// Check the type of line
		if (chars[0] == 'v' && chars[1] == 'n')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			glm::vec3 norm;
			sscanf_s(
				chars,
				"vn %f %f %f",
				&norm.x, &norm.y, &norm.z);

			// Add to the list of normals
			normals.push_back(norm);
		}
		else if (chars[0] == 'v' && chars[1] == 't')
		{
			// Read the 2 numbers directly into an XMFLOAT2
			glm::vec2 uv;
			sscanf_s(
				chars,
				"vt %f %f",
				&uv.x, &uv.y);

			// Add to the list of uv's
			uvs.push_back(uv);
		}
		else if (chars[0] == 'v')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			glm::vec3 pos;
			sscanf_s(
				chars,
				"v %f %f %f",
				&pos.x, &pos.y, &pos.z);

			// Add to the positions
			positions.push_back(pos);
		}
		else if (chars[0] == 'f')
		{
			// Read the face indices into an array
			unsigned int i[12];
			int facesRead = sscanf_s(
				chars,
				"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
				&i[0], &i[1], &i[2],
				&i[3], &i[4], &i[5],
				&i[6], &i[7], &i[8],
				&i[9], &i[10], &i[11]);

			// - Create the verts by looking up
			//    corresponding data from vectors
			// - OBJ File indices are 1-based, so
			//    they need to be adusted
			Vertex v1;
			v1.position = positions[i[0] - 1];
			v1.uv = uvs[i[1] - 1];
			v1.normal = normals[i[2] - 1];

			Vertex v2;
			v2.position = positions[i[3] - 1];
			v2.uv = uvs[i[4] - 1];
			v2.normal = normals[i[5] - 1];

			Vertex v3;
			v3.position = positions[i[6] - 1];
			v3.uv = uvs[i[7] - 1];
			v3.normal = normals[i[8] - 1];

			// Flip the UV's since they're probably "upside down"
			v1.uv.y = 1.0f - v1.uv.y;
			v2.uv.y = 1.0f - v2.uv.y;
			v3.uv.y = 1.0f - v3.uv.y;

			// Add the verts to the vector
			verts.push_back(v1);
			verts.push_back(v2);
			verts.push_back(v3);

			// Add three more indices
			indices.push_back(vertCounter); vertCounter += 1;
			indices.push_back(vertCounter); vertCounter += 1;
			indices.push_back(vertCounter); vertCounter += 1;

			// Was there a 4th face?
			if (facesRead == 12)
			{
				// Make the last vertex
				Vertex v4;
				v4.position = positions[i[9] - 1];
				v4.uv = uvs[i[10] - 1];
				v4.normal = normals[i[11] - 1];

				// Flip the y
				v4.uv.y = 1.0f - v4.uv.y;

				// Add a whole triangle
				verts.push_back(v1);
				verts.push_back(v3);
				verts.push_back(v4);

				// Add three more indices
				indices.push_back(vertCounter); vertCounter += 1;
				indices.push_back(vertCounter); vertCounter += 1;
				indices.push_back(vertCounter); vertCounter += 1;
			}
		}

	}
	// Close the file and create the actual buffers
	obj.close();
}
//calls loadObj, then passes the vertex data to the GPU for us. Sets the location parameters 
//in lieu of a return value.
void setupMesh(const char* filename, GLuint &vao, GLuint &vbo, GLuint &ibo, GLuint &numIndices)
{
	std::vector<Vertex> verts;
	std::vector<UINT> indices;
	loadObj(filename, verts,indices);

	//creat OpenGL objects to use in drawing
	unsigned int gl_vertex_array_object, gl_vertex_buffer_object, gl_index_buffer_object;

	//vertex array object 
	glGenVertexArrays(1, &gl_vertex_array_object);
	glBindVertexArray(gl_vertex_array_object);

	//vertex buffer object holds the vertices 
	glGenBuffers(1, &gl_vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), &verts[0], GL_STATIC_DRAW);

	//index buffer object holds the index of the vertex
	glGenBuffers(1, &gl_index_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_index_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


	//the connection between attributes, interleaved data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);                       //send positions on pipe 0
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));       //send normals on pipe 1
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 6));     //send uv on pipe 2


	vao = gl_vertex_array_object;
	vbo = gl_vertex_buffer_object;
	ibo = gl_index_buffer_object;
	numIndices = indices.size();
}