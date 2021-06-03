#include "pch.h"
#include "glWindow.h"

glWindow::glWindow()
{
    bufferWidth = 1000;
    bufferHeight = 770;
    deltax = 0;
    deltay = 0;
    firstMove = true;
}

glWindow::glWindow( unsigned int width, unsigned int height)
{
    bufferWidth = width;
    bufferHeight = height;
    deltax = 0;
    deltay = 0;
    firstMove = true;
}

glWindow::~glWindow()
{
    glfwDestroyWindow( mainWindow );
    glfwTerminate();
}

void glWindow::HandleInput( GLFWwindow* window , int key , int scancode , int action , int mods )
{
    if ( key < 0 || key >= 1024 )
        return;

    glWindow* MainWindow = static_cast< glWindow* >( glfwGetWindowUserPointer( window ) );
    
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
        glfwSetWindowShouldClose( window , GL_TRUE );

    if ( action == GLFW_PRESS )
        MainWindow->keys[ key ] = true;
    if (action == GLFW_RELEASE )
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

void glWindow::HandleScroll( GLFWwindow* window , double xoffset , double yoffset )
{
    glWindow* MainWindow = static_cast< glWindow* >( glfwGetWindowUserPointer(window) );

    MainWindow->scrollDelta = yoffset;
}

void glWindow::HandleButton( GLFWwindow* window , int button , int action , int mods )
{
    if ( button < 0 || button >= 8 )
        return;

    glWindow* MainWindow = static_cast< glWindow* >( glfwGetWindowUserPointer( window ) );

    if ( button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS )
    {
        glfwSetInputMode( MainWindow->mainWindow , GLFW_CURSOR , GLFW_CURSOR_DISABLED );
        MainWindow->firstMove = true;
    }
    else 
    {
        glfwSetInputMode( MainWindow->mainWindow , GLFW_CURSOR , GLFW_CURSOR_NORMAL );
    }

    if ( action == GLFW_PRESS )
        MainWindow->buttons[ button ] = true;
    if ( action == GLFW_RELEASE )
        MainWindow->buttons[ button ] = false;
}

void glWindow::CreateCallbacks()
{
    glfwSetKeyCallback( mainWindow , HandleInput );
    glfwSetCursorPosCallback( mainWindow , HandeCursor );
    glfwSetScrollCallback( mainWindow , HandleScroll );
    glfwSetMouseButtonCallback( mainWindow , HandleButton );
}

GLfloat glWindow::GetMouseDeltaX()
{
    float delta = deltax;
    deltax = 0;
    return delta;
}

GLfloat glWindow::GetMouseDeltaY()
{
    float delta = deltay;
    deltay = 0;
    return delta;
}

GLfloat glWindow::GetScrollDelta()
{
    float delta = scrollDelta;
    scrollDelta = 0;
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
    glfwSetInputMode( mainWindow , GLFW_CURSOR , GLFW_CURSOR_NORMAL );

    glewExperimental = GL_TRUE;

    if ( glewInit() != GLEW_OK )
        printf( "Failed to initialize GLEW\n" );

    printf( "%s\n" , glGetString( GL_VERSION ) );

    glEnable( GL_DEPTH_TEST );

    glViewport( 0 , 0 , bufferWidth , bufferHeight );

    glfwSetWindowUserPointer( mainWindow , this );

    glPolygonMode( GL_FRONT_AND_BACK , GL_FILL );
    
}