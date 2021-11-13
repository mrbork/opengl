#pragma once
#include "pch.h"

class Lighting
{
public:
	Lighting();
	Lighting( GLfloat red , GLfloat green , GLfloat blue , GLfloat ambientintensity , GLfloat diffuseintensity);

	void Use( unsigned int ambientColorLocation , unsigned int ambientIntensityLocation, unsigned int diffuseIntensityLocation);

	void Update( glm::vec3 ambientColor , float ambientIntensity, glm::vec3 lightDirection );

	~Lighting();

protected:
	glm::vec3 ambientColor;

	float ambientIntensity;
	float diffuseIntensity;
};

