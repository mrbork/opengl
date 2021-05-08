#include "glWindow.h"

glWindow::glWindow()
{
    bufferWidth = 700;
    bufferHeight = 500;
}

glWindow::glWindow( unsigned int width, unsigned int height)
{
    bufferWidth = width;
    bufferHeight = height;
}

void glWindow::Initialize()
{
    /* Initialize the library */
    if ( !glfwInit() ) {
        printf( "Failed to initialize GLFW." );
        glfwTerminate();
        return;
    }

    /* Create a windowed mode window and its OpenGL context */
    mainWindow = glfwCreateWindow( bufferWidth , bufferHeight , "OpenGL" , 0 , 0 );
    if ( !mainWindow )
    {
        glfwTerminate();
        return;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent( mainWindow );

    glewExperimental = GL_TRUE;

    if ( glewInit() != GLEW_OK )
        printf( "Failed to initialize GLEW\n" );

    printf( "%s\n" , glGetString( GL_VERSION ) );

    glEnable( GL_DEPTH_TEST );

}