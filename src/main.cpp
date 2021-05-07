#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "Mesh.h"

#define toRadians 3.141592653589f / 180.0f

std::vector<Mesh*> meshList;

void CreateTriangle() {

    float vertices[] {
        -0.5f,-0.5f, 0.0f, //Left
         0.5f,-0.5f, 0.0f, //Right
         0.0f, 0.5f, 0.0f, //Top
         0.0f, 0.0f, 0.5f  //Z
    };

    unsigned int indices[]{
        0, 1, 2,
        3, 2, 1,
        3, 2, 0,
        3, 0, 1
    };

    Mesh* object1 = new Mesh();
    object1->CreateMesh( vertices , sizeof( vertices ) / sizeof( float ) , indices , sizeof( indices ) / sizeof( unsigned ) );

    Mesh* object2 = new Mesh();
    object2->CreateMesh( vertices , sizeof( vertices ) / sizeof( float ) , indices , sizeof( indices ) / sizeof( unsigned ) );

    meshList.push_back( object1 );
    meshList.push_back( object2 );

}

unsigned int CompileShader(unsigned int type, const std::string& src) {

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
        printf( "Failed to compile the %d shader\n %s\n" , type, message );
        return 0;
    }

    return shader;
}

unsigned int CompileProgram() {

    unsigned int shaders = glCreateProgram();

    if ( !shaders )
        printf( "Failed to create shader program.\n" );

    std::ifstream vs_s("Shaders/vertex.shader");
    const std::string vs_src = std::string(std::istreambuf_iterator<char>(vs_s), std::istreambuf_iterator<char>());

    std::ifstream fs_s("Shaders/fragment.shader");
    const std::string fs_src = std::string(std::istreambuf_iterator<char>(fs_s), std::istreambuf_iterator<char>());

    unsigned vertexShader = CompileShader( GL_VERTEX_SHADER , vs_src );
    unsigned fragmentShader = CompileShader(GL_FRAGMENT_SHADER , fs_src);

    glAttachShader(shaders, vertexShader);
    glAttachShader(shaders, fragmentShader);

    int result;
    char message[1024];

    glLinkProgram( shaders );
    glGetProgramiv( shaders , GL_LINK_STATUS , &result );
    if ( !result ) 
    {
        glGetProgramInfoLog( shaders , 1024 , NULL , message );
        printf( "Failed to link program: %s\n" , message );
        return 0;
    }

    glValidateProgram( shaders );
    glGetProgramiv( shaders , GL_VALIDATE_STATUS , &result );
    if ( !result )
    {
        glGetProgramInfoLog( shaders , 1024 , NULL , message );
        printf( "Failed to validate program: %s\n" , message );
        return 0;
    }

    glDetachShader(shaders, vertexShader);
    glDetachShader(shaders, fragmentShader);

    return shaders;
}


int main()
{
    unsigned int uniformModel , uniformProjection;
    float uXoffset = 0.0f;
    bool direction = true;

    /* Initialize the library */
    if ( !glfwInit() ) {
        printf( "Failed to initialize GLFW." );
        glfwTerminate();
        return 1;
    }

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow( 700 , 500 , "OpenGL" , NULL , NULL );
    if ( !window )
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent( window );

    glewExperimental = GL_TRUE;

    if ( glewInit() != GLEW_OK )
        printf( "Failed to initialize GLEW\n" );

    printf( "%s\n" , glGetString( GL_VERSION ) );

    glEnable( GL_DEPTH_TEST );

    CreateTriangle();
    unsigned int shaders = CompileProgram();

    uniformModel = glGetUniformLocation( shaders , "model" );
    uniformProjection = glGetUniformLocation( shaders , "projection" );

    glm::mat4 projectionMatrix = glm::perspective( 90.0f , 700.0f / 500.0f , 0.1f , 100.0f );


    /* Loop until the user closes the window */
    while ( !glfwWindowShouldClose( window ) )
    {
        /* Poll for and process events */
        glfwPollEvents();

        uXoffset += ( direction ) ? 0.0025f : -0.0025f;

        if ( abs( uXoffset ) > 0.9f )
            direction = !direction;

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram( shaders );

        /*Transformations*/
        glm::mat4 transformMatrix(1.0f);
        transformMatrix = glm::translate( transformMatrix , glm::vec3( uXoffset , 0.0f , -1.0f ) );
        //transformMatrix = glm::rotate(transformMatrix, uXoffset * 100.f * toRadians, glm::vec3(0.0f, 0.0f, 0.0f));
        //transformMatrix = glm::scale(transformMatrix, glm::vec3(uXoffset, uXoffset, 1.0f));
        

        /*Uniforms*/
        glUniformMatrix4fv( uniformModel , 1 , GL_FALSE , glm::value_ptr( transformMatrix ) );
        glUniformMatrix4fv( uniformProjection , 1 , GL_FALSE , glm::value_ptr( projectionMatrix ) );


        meshList[ 0 ]->RenderMesh();
        meshList[ 1 ]->RenderMesh();

        glUseProgram( 0 );

        /*Check for Errors*/
        GLenum err;
        while ( ( err = glGetError() ) != GL_NO_ERROR )
        {
            //printf( "OpenGL Error: %d\n" , err );
            std::cout << std::hex << "OpenGL Error: 0x" << err << std::endl;
        }

        glfwSwapBuffers( window );

    }

    meshList[ 0 ]->ClearMesh();
    meshList[ 1 ]->ClearMesh();
    glDeleteProgram( shaders );
    glfwTerminate();

    return 0;
}