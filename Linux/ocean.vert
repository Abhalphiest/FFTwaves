#version 330

//
// File: ocean.vert
// Author: Margaret Dorsey
// 
// The vertex shader for our
// FFT ocean


//the usual..
//texture = uv coordinate
in vec3 vertex;
in vec3 normal;
in vec3 texture;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform vec3 light_position;

//output for FS for lighting calcs
out vec3 light_vector;
out vec3 normal_vector;
out vec3 halfway_vector;
out float fog_factor;
out vec2 tex_coord;

void main() {
	//transform our vertex into world coords
	gl_Position = View * Model * vec4(vertex, 1.0);
	//get our world coord position (NOT PROJECTED) to see how foggy it should be
	fog_factor = min(-gl_Position.z/800.0, 1.0);
	//now we apply the projection
	gl_Position = Projection * gl_Position;

	//do the same multiplication as before 
	//(kind of a waste, but this shader wasn't planned out great)
	vec4 v = View * Model * vec4(vertex, 1.0);
	vec3 normal1 = normalize(normal);

	//get the vector from our light to the vertex
	light_vector = normalize((View * vec4(light_position, 1.0)).xyz - v.xyz);
	//transform our normal vector
	normal_vector = (inverse(transpose(View * Model)) * vec4(normal1, 0.0)).xyz;
	//get halfway vector to simulate reflection
        halfway_vector = light_vector + normalize(-v.xyz);

	//pass in uv coordinate
	tex_coord = texture.xy;
} 