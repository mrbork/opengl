#include "Shaders.h"

Shaders::Shaders()
{
    vertexShader = 0;
    fragmentShader = 0;
    program = 0;
}

unsigned int Shaders::CompileShader( unsigned int type , const std::string& src )
{
    const char* source = src.c_str();

    unsigned int shader = glCreateShader( type );
    glShaderSource( shader , 1 , &source , nullptr );
    glCompileShader( shader );

    int result;
    char message[ 1024 ];

    glGetShaderiv( shader , GL_COMPILE_STATUS , &result );
    if ( !result )
    {
        glGetShaderInfoLog( shader , 1024 , NULL , message );
        printf( "Failed to compile the %d shader\n %s\n" , type , message );
        return 0;
    }

    return shader;
}

void Shaders::CompileProgram()
{
    program = glCreateProgram();

    if ( !program )
        printf( "Failed to create shader program.\n" );

    std::ifstream vs_s( "Shaders/vertex.shader" );
    const std::string vs_src = std::string( std::istreambuf_iterator<char>( vs_s ) , std::istreambuf_iterator<char>() );

    std::ifstream fs_s( "Shaders/fragment.shader" );
    const std::string fs_src = std::string( std::istreambuf_iterator<char>( fs_s ) , std::istreambuf_iterator<char>() );

    vertexShader = CompileShader( GL_VERTEX_SHADER , vs_src );
    fragmentShader = CompileShader( GL_FRAGMENT_SHADER , fs_src );

    if ( !vertexShader )
        printf( "Failed to compile Vertex Shader\n" );

    if ( !fragmentShader )
        printf( "Failed to compile Fragment Shader\n" );

    glAttachShader( program , vertexShader );
    glAttachShader( program , fragmentShader );

    int result;
    char message[ 1024 ];

    glLinkProgram( program );
    glGetProgramiv( program , GL_LINK_STATUS , &result );
    if ( !result )
    {
        glGetProgramInfoLog( program , 1024 , NULL , message );
        printf( "Failed to link program: %s\n" , message );
        return;
    }

    glValidateProgram( program );
    glGetProgramiv( program , GL_VALIDATE_STATUS , &result );
    if ( !result )
    {
        glGetProgramInfoLog( program , 1024 , NULL , message );
        printf( "Failed to validate program: %s\n" , message );
        return;
    }

    glDetachShader( program , vertexShader );
    glDetachShader( program , fragmentShader );

}

void Shaders::ClearShader()
{
    glDeleteProgram( program );

    vertexShader = 0;
    fragmentShader = 0;
    program = 0;
}