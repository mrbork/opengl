#include "pch.h"
#include "Shaders.h"

Shaders::Shaders()
{
    vertexShader = 0;
    fragmentShader = 0;
    program = 0;

    uniformpointLightCount = 0;
}

unsigned int Shaders::CompileShader( unsigned int type , const std::string& src )
{
    const char* source = src.c_str();

    unsigned int shader = glCreateShader( type );
    glShaderSource( shader , 1 , &source , nullptr );
    glCompileShader( shader );

    int result;
    char message[ 512 ];

    glGetShaderiv( shader , GL_COMPILE_STATUS , &result );
    if ( !result )
    {
        glGetShaderInfoLog( shader , 512 , NULL , message );
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
    char message[ 512 ];

    glLinkProgram( program );
    glGetProgramiv( program , GL_LINK_STATUS , &result );
    if ( !result )
    {
        glGetProgramInfoLog( program , 512 , NULL , message );
        printf( "Failed to link program: %s\n" , message );
        return;
    }

    glValidateProgram( program );
    glGetProgramiv( program , GL_VALIDATE_STATUS , &result );
    if ( !result )
    {
        glGetProgramInfoLog( program , 512 , NULL , message );
        printf( "Failed to validate program: %s\n" , message );
        return;
    }

    glDetachShader( program , vertexShader );
    glDetachShader( program , fragmentShader );

    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

    uniformModel = glGetUniformLocation( program , "transformation" );
    uniformProjection = glGetUniformLocation( program , "projection" );
    uniformView = glGetUniformLocation( program , "view" );
    uniformCameraPosition = glGetUniformLocation( program , "cameraPos" );
    uniformSpecularIntensity = glGetUniformLocation( program , "material.specularIntensity" );
    uniformShininess = glGetUniformLocation( program , "material.Shininess" );

    directionalLight.uniformColor = glGetUniformLocation( program , "directionalLight.base.color" );
    directionalLight.uniformambientIntensity = glGetUniformLocation( program , "directionalLight.base.ambientIntensity" );
    directionalLight.uniformdiffuseIntensity = glGetUniformLocation( program , "directionalLight.base.diffuseIntensity" );
    directionalLight.uniformDirection = glGetUniformLocation( program , "directionalLight.direction" );

    uniformpointLightCount = glGetUniformLocation( program , "pointLightCount" );

    for ( int i = 0; i < MAX_POINTLIGHT_COUNT; i++ )
    {
        char locBuffer[ 100 ] = { '\0' };

        snprintf( locBuffer , sizeof( locBuffer ) , "pointLight[%d].base.color" , i );
        pointLight[ i ].uniformColor = glGetUniformLocation( program , locBuffer );

        snprintf( locBuffer , sizeof( locBuffer ) , "pointLight[%d].base.ambientIntensity" , i );
        pointLight[ i ].uniformambientIntensity = glGetUniformLocation( program , locBuffer );

        snprintf( locBuffer , sizeof( locBuffer ) , "pointLight[%d].base.diffuseIntensity" , i );
        pointLight[ i ].uniformdiffuseIntensity = glGetUniformLocation( program , locBuffer );

        snprintf( locBuffer , sizeof( locBuffer ) , "pointLight[%d].position" , i );
        pointLight[ i ].uniformPosition = glGetUniformLocation( program , locBuffer );

        snprintf( locBuffer , sizeof( locBuffer ) , "pointLight[%d].constant" , i );
        pointLight[ i ].uniformConstant = glGetUniformLocation( program , locBuffer );

        snprintf( locBuffer , sizeof( locBuffer ) , "pointLight[%d].linear" , i );
        pointLight[ i ].uniformLinear = glGetUniformLocation( program , locBuffer );

        snprintf( locBuffer , sizeof( locBuffer ) , "pointLight[%d].exponent" , i );
        pointLight[ i ].uniformExponent = glGetUniformLocation( program , locBuffer );
    }
}

void Shaders::SetDirectionalLight( DirectionalLight* dLight )
{
    dLight->Use( directionalLight.uniformColor , directionalLight.uniformambientIntensity , directionalLight.uniformdiffuseIntensity , directionalLight.uniformDirection );
}

void Shaders::SetPointLight( PointLight* pLight , unsigned int pointLightCount )
{
    if ( pointLightCount > MAX_POINTLIGHT_COUNT ) pointLightCount = MAX_POINTLIGHT_COUNT;
    glUniform1i( uniformpointLightCount , pointLightCount );

    for ( unsigned int i = 0; i < pointLightCount; i++ )
    {
        pLight[ i ].Use( pointLight[ i ].uniformColor , pointLight[ i ].uniformambientIntensity , pointLight[ i ].uniformdiffuseIntensity , pointLight[ i ].uniformPosition , pointLight[ i ].uniformConstant , pointLight[ i ].uniformLinear , pointLight[ i ].uniformExponent );
    }
}

void Shaders::ClearShader()
{
    glDeleteProgram( program );

    vertexShader = 0;
    fragmentShader = 0;
    program = 0;
}