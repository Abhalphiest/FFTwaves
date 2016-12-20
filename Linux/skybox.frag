#version 330 core
in vec3 TexCoords;
out vec4 color;

uniform samplerCube skybox;

void main()
{    
	vec3 coord = normalize(TexCoords);
    color = texture(skybox, coord);
	//color = vec4(1.0,0.0,0.0,1.0);
}