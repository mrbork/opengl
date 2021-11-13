#include "pch.h"

class Material
{
public: 
	Material();
	Material( GLfloat specularintensity , GLfloat shininess );

	void UseMaterial( GLuint specularIntensityLocation , GLuint ShininessLocation );

	~Material();

private:
	GLfloat specularIntensity;
	GLfloat Shininess;
};

