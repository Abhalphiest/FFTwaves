#version 330 core
//File:	objects.frag
//Author: Margaret Dorsey
//
//the fragment shader for drawing the pier/dock and 
// teapot


out vec4 color; //final color of this fragment on the screen

uniform sampler2D Texture;

in vec2 UV;
in vec3 Normal;
in vec3 light_vector;

void main()
{    
	vec3 normal1         = normalize(Normal);
	vec3 light_vector1   = normalize(light_vector);

	vec4 c = texture(Texture, UV);
	
	//these are closer to grays than the ocean values.
	//would not normally be hard coded in a larger project.
	vec4 ambient_color  = vec4(0.6, 0.65, 0.75, 1.0);
	vec4 diffuse_color  = vec4(0.7, 0.75, 0.95, 1.0);

	float ambient_contribution  = 0.50;
	float diffuse_contribution  = 1.70;

	//get the cosine of the angle between the vector to the light
	//and the normal of our face/vertex
	float d = dot(normal1, light_vector1);
	bool facing = d > 0.0;

	//diffuse color is scaled by how direct the light is
	//ambient color simulates ambient light in a scene that would
	//physically be the result of reflection/refraction that we
	//do not calculate
	color = 
		    ambient_color  * ambient_contribution  * c +
		    diffuse_color  * diffuse_contribution  * c * max(d, 0);


	color.a = 1.0;
}