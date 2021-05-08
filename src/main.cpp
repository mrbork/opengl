#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shaders.h"
#include "glWindow.h" 

#define toRadians 3.141592653589f / 180.0f

std::vector<Mesh*> meshlist;
std::vector<Shaders> shaderlist;
glWindow mainWindow;

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
    Mesh* object2 = new Mesh();

    object1->CreateMesh( vertices , sizeof( vertices ) / sizeof( float ) , indices , sizeof( indices ) / sizeof( unsigned ) );
    object2->CreateMesh( vertices , sizeof( vertices ) / sizeof( float ) , indices , sizeof( indices ) / sizeof( unsigned ) );

    meshlist.push_back( object1 );
    meshlist.push_back( object2 );

}

int main()
{
    mainWindow.Initialize();

    unsigned int uniformModel , uniformProjection;
    shaderlist.push_back( Shaders() );

    CreateTriangle();
    shaderlist[ 0 ].CompileProgram();

    uniformModel = glGetUniformLocation( shaderlist[0].GetProgram() , "model" );
    uniformProjection = glGetUniformLocation( shaderlist[ 0 ].GetProgram() , "projection" );

    glm::mat4 projectionMatrix = glm::perspective(90.0f , 700.0f / 500.0f , 0.1f , 100.0f );


    while ( !mainWindow.ShouldClose() )
    {
        /* Poll for and process events */
        glfwPollEvents();

        glClearColor( 0.15f , 0.17f , 0.20f , 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderlist[ 0 ].UseShader();
        
        glUniformMatrix4fv( uniformProjection , 1 , GL_FALSE , glm::value_ptr( projectionMatrix ) );


        /*Transformation*/
        glm::mat4 transformMatrix( 1.0f );
        transformMatrix = glm::translate( transformMatrix , glm::vec3( 0.0f , 0.0f , -1.0f ) );
        glUniformMatrix4fv( uniformModel , 1 , GL_FALSE , glm::value_ptr( transformMatrix ) );

        meshlist[ 0 ]->RenderMesh();


        transformMatrix = glm::mat4(1.0f);
        transformMatrix = glm::translate( transformMatrix , glm::vec3( 0.0f , 1.0f , -1.0f ) );
        glUniformMatrix4fv( uniformModel , 1 , GL_FALSE , glm::value_ptr( transformMatrix ) );

        meshlist[ 1 ]->RenderMesh();

        /*Check for Errors*/
        GLenum err;
        while ( ( err = glGetError() ) != GL_NO_ERROR )
            std::cout << std::hex << "OpenGL Error: 0x" << err << std::endl;

        mainWindow.SwapBuffers();

    }

    meshlist[ 0 ]->ClearMesh();
    meshlist[ 1 ]->ClearMesh();
    shaderlist[ 0 ].ClearShader();
    
    mainWindow.Clear();

    return 0;
}