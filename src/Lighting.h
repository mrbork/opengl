#include "pch.h"

class Lighting
{
public:
	Lighting();
	Lighting( GLfloat red , GLfloat green , GLfloat blue , GLfloat ambientintensity , GLfloat dirX , GLfloat dirY , GLfloat dirZ , GLfloat diffuseintensity );

	void Use( unsigned int ambientColorLocation , unsigned int ambientIntensityLocation, unsigned int directionLocation, unsigned int diffuseIntensityLocation );

	void Update( glm::vec3 ambientColor , float ambientIntensity );

	~Lighting();

private:
	glm::vec3 ambientColor;
	float ambientIntensity;

	glm::vec3 direction;
	float diffuseIntensity;
};

