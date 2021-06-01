#include "pch.h"

class Shaders
{
public:
	Shaders();

	void CompileProgram();

	void UseShader() { glUseProgram( program ); }

	void ClearShader();

	GLuint GetProgram() { return program; }
private:

	unsigned int CompileShader( unsigned int type , const std::string& src );

	unsigned vertexShader;
	unsigned fragmentShader;
	unsigned int program;
};

