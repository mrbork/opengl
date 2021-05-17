#version 330 core

out vec4 gl_FragColor;

in vec4 vertexColor;
in vec2 texelCoord;

uniform sampler2D textureSampler;

void main()
{
   gl_FragColor = texture(textureSampler, texelCoord);
}