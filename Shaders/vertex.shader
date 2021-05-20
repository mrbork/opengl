#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec4 vertexColor;
out vec2 texelCoord;

uniform mat4 transformation;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * transformation * vec4( position, 1.0f );
	vertexColor = vec4(clamp(position, 0.0f, 1.0f), 1.0f);
	
	texelCoord = texCoord;
}