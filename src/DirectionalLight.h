#include "Lighting.h"

class DirectionalLight : public Lighting
{
public:
	DirectionalLight();

	DirectionalLight( GLfloat red , GLfloat green , GLfloat blue , GLfloat ambientIntensity , GLfloat diffuseIntensity, GLfloat dirX , GLfloat dirY , GLfloat dirZ );

	void Use( unsigned int ambientColorLocation , unsigned int ambientIntensityLocation , unsigned int diffuseIntensityLocation, unsigned int directionLocation );

	~DirectionalLight();

private:
	glm::vec3 direction;
};

