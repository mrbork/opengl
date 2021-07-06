#include "Lighting.h"

class PointLight : public Lighting
{
public:
	PointLight();

	PointLight( GLfloat red , GLfloat green , GLfloat blue , GLfloat ambientIntensity , GLfloat diffuseIntensity , GLfloat posX , GLfloat posY , GLfloat posZ , GLfloat cont , GLfloat lin , GLfloat exp );

	void Use( unsigned int ambientColorLocation , unsigned int ambientIntensityLocation , unsigned int diffuseIntensityLocation , unsigned int positionLocation , unsigned int constantLocation , unsigned int linearLocation , unsigned int exponentLocation );

	~PointLight();

private:
	glm::vec3 position;

	float constant , linear , exponent;
};

