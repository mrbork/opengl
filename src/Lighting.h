#include <GL/glew.h>
#include <GLM/glm.hpp>

class Lighting
{
public:
	Lighting();
	Lighting( GLfloat red , GLfloat green , GLfloat blue , GLfloat ambientIntensity );

	void Use( unsigned int ambientColorLocation , unsigned int ambientIntensityLocation );

	void Update( glm::vec3 ambientColor , float ambientIntensity );

	~Lighting();

private:
	glm::vec3 color;
	float intensity;
};

