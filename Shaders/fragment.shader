#version 330 core

in vec4 vertexColor;
in vec2 texelCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 gl_FragColor;

const int MAX_POINTLIGHT_COUNT = 3;

struct Light
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct Material
{
	float specularIntensity;
	float Shininess;
};

uniform int pointLightCount;

uniform sampler2D textureSampler;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight[MAX_POINTLIGHT_COUNT];
uniform Material material;

uniform vec3 cameraPos;

vec4 CalcLightbyDirection(Light light, vec3 direction)
{
	vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
	
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
	vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;
	
	vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	if (diffuseFactor > 0.0f)
	{
		vec3 EyetoFrag = normalize(cameraPos - FragPos);
		vec3 reflectedLight = normalize(reflect(direction, normalize(Normal)));
		
		float specularFactor = dot(EyetoFrag, reflectedLight);
		
		if (specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.Shininess);
			specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	
	return (ambientColor + diffuseColor + specularColor);
}

vec4 CalcDirectionalLight()
{
	return CalcLightbyDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLight()
{
	vec4 totalColor = vec4(0, 0, 0, 0);
	
	for (int i = 0; i < pointLightCount; i++)
	{
		vec3 direction = FragPos - pointLight[i].position;
		float distance = length(direction);
		direction = normalize(direction);
		
		vec4 color = CalcLightbyDirection(pointLight[i].base, direction);
		float attenuation = pointLight[i].exponent * distance * distance + pointLight[i].linear * distance + pointLight[i].constant;
		
		totalColor += color / attenuation;
	}
	
	return totalColor;
}

void main()
{
	vec4 finalColor = CalcDirectionalLight() + CalcPointLight();
	
	
	gl_FragColor = texture(textureSampler, texelCoord) * finalColor;
}