#include "pch.h"

#include "DirectionalLight.h"	
#include "PointLight.h"

class Shaders
{
public:
	Shaders();

	void CompileProgram();

	void UseShader() { glUseProgram( program ); }

	void ClearShader();

	void SetDirectionalLight( DirectionalLight* dLight );
	void SetPointLight( PointLight* pLight , unsigned int pointLightCount );

	GLuint GetModel() { return uniformModel; }
	GLuint GetProjection() { return uniformProjection; }
	GLuint GetView() { return uniformView; }
	GLuint GetCameraPosition() { return uniformCameraPosition; }
	GLuint GetSpecularIntensity() { return uniformSpecularIntensity; }
	GLuint GetShininess() { return uniformShininess; }

private:

	unsigned int CompileShader( unsigned int type , const std::string& src );

	unsigned vertexShader;
	unsigned fragmentShader;
	unsigned int program;

	unsigned int uniformModel , uniformProjection , uniformView , uniformCameraPosition , uniformSpecularIntensity , uniformShininess;

	unsigned int uniformpointLightCount;

	struct {
		unsigned int uniformColor;
		unsigned int uniformambientIntensity;
		unsigned int uniformdiffuseIntensity;

		unsigned int uniformDirection;
	} directionalLight;

	struct {
		unsigned int uniformColor;
		unsigned int uniformambientIntensity;
		unsigned int uniformdiffuseIntensity;

		unsigned int uniformPosition;
		unsigned int uniformConstant;
		unsigned int uniformLinear;
		unsigned int uniformExponent;
	} pointLight[MAX_POINTLIGHT_COUNT];


};

