#version 330 core

out vec4 gl_FragColor;

in vec4 vertexColor;
in vec2 texelCoord;

struct DirectionalLight
{
	vec3 color;
	float intensity;
};

uniform sampler2D textureSampler;
uniform DirectionalLight directionalLight;

void main()
{
	vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.intensity;
	
	gl_FragColor = texture(textureSampler, texelCoord) * ambientColor;
}