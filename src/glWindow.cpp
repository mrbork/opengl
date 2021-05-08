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

glWindow::~glWindow()
{
    glfwDestroyWindow( mainWindow );
    glfwTerminate();
}

void glWindow::HandleInput( GLFWwindow* window , int key , int scancode , int action , int mods )
{
    if ( key < 0 || key > 1024 )
        return;

    glWindow* MainWindow = static_cast< glWindow* >( glfwGetWindowUserPointer( window ) );
    
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
        glfwSetWindowShouldClose( window , GL_TRUE );

    if ( action == GLFW_PRESS )
        MainWindow->keys[ key ] = true;
    else
        MainWindow->keys[ key ] = false;
}

void glWindow::HandeCursor( GLFWwindow* window , double xPos , double yPos )
{
    glWindow* MainWindow = static_cast< glWindow* >( glfwGetWindowUserPointer( window ) );

    if ( MainWindow->firstMove )
    {
        MainWindow->lastx = xPos;
        MainWindow->lasty = yPos;
        MainWindow->firstMove = false;
    }

    MainWindow->deltax = xPos - MainWindow->lastx;
    MainWindow->deltay = MainWindow->lasty - yPos;

    MainWindow->lastx = xPos;
    MainWindow->lasty = yPos;

}

void glWindow::CreateCallbacks()
{
    glfwSetKeyCallback( mainWindow , HandleInput );
    glfwSetCursorPosCallback( mainWindow , HandeCursor );
}

GLfloat glWindow::GetDeltaX()
{
    float delta = deltax;
    deltax = 0;
    return delta;
}

GLfloat glWindow::GetDeltaY()
{
    float delta = deltay;
    deltay = 0;
    return delta;
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

    /*Handle inputs*/
    CreateCallbacks();
    
    /*Hide cursor and set it to origin*/
    glfwSetInputMode( mainWindow , GLFW_CURSOR , GLFW_CURSOR_DISABLED );

    glewExperimental = GL_TRUE;

    if ( glewInit() != GLEW_OK )
        printf( "Failed to initialize GLEW\n" );

    printf( "%s\n" , glGetString( GL_VERSION ) );

    glEnable( GL_DEPTH_TEST );

    glViewport( 0 , 0 , bufferWidth , bufferHeight );

    glfwSetWindowUserPointer( mainWindow , this );

}