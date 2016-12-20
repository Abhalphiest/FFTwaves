#version 330

//File:	objects.vert
//Author: Margaret Dorsey
//
//the vertex shader for drawing the pier/dock and 
// teapot

 
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
 
//uniforms
uniform mat4 Model; 
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 light_position;

out vec2 UV;
out vec3 Normal;
out vec3 light_vector;

 
void main()
{
	//nothing interesting here, apply our transformations and
	//pass data to the pixel shader.
	vec4 pos = vec4(position,1.0);
	mat4 mvp = Projection*View*Model;
	gl_Position = mvp*pos;
	UV = uv;
	Normal = normal;
	light_vector = normalize((View * vec4(light_position, 1.0)).xyz - position.xyz);
}