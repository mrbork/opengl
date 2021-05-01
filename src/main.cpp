#include <iostream>
#include <fstream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

const float toRadians = 3.141592653589f / 180.0f;

void CreateTriangle(unsigned int& vao, unsigned int& vbo) {

    float vertices[] {
        -0.5f,-0.5f,
         0.5f,-0.5f,
         0.0f, 0.5f
    };

    glGenVertexArrays( 1 , &vao );
    glBindVertexArray( vao );

    glGenBuffers( 1 , &vbo );
    glBindBuffer( GL_ARRAY_BUFFER , vbo );
    glBufferData( GL_ARRAY_BUFFER , sizeof(vertices), vertices, GL_STATIC_DRAW );
    
    glVertexAttribPointer( 0 , 2 , GL_FLOAT , GL_FALSE , 0 , 0 );
    glEnableVertexAttribArray( 0 );

    //Unbind
    glBindBuffer( GL_ARRAY_BUFFER , 0 );
    glBindVertexArray( 0 );
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
    unsigned int vao , vbo , uniformModel, uniformColor;
    float uXoffset = 0.0f , uXincrement = 0.0025f;
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

    CreateTriangle(vbo, vao);
    unsigned int shaders = CompileProgram();

    uniformModel = glGetUniformLocation( shaders , "model" );
    uniformColor = glGetUniformLocation( shaders , "colorrgb" );

    glm::vec3 color{ 255.0f , 0.0f , 0.0f };
    float counter = 0;

    /* Loop until the user closes the window */
    while ( !glfwWindowShouldClose( window ) )
    {
        /* Poll for and process events */
        glfwPollEvents();

        uXoffset += ( direction ) ? uXincrement : -uXincrement;
        counter++;

        if ( abs( uXoffset ) > 0.9f )
            direction = !direction;

        if ( color[ 0 ] > 0 && color[ 2 ] == 0 ) {
            color[ 0 ]--;
            color[ 1 ]++;
        }
        else
            if ( color[ 1 ] > 0 && color[ 0 ] == 0 ) {
                color[ 1 ]--;
                color[ 2 ]++;
            }
            else
                if ( color[ 2 ] > 0 && color[ 1 ] == 0 ) {
                    color[ 0 ]++;
                    color[ 2 ]--;
                }
        color[ 0 ] /= 255.0f;
        color[ 1 ] /= 255.0f;
        color[ 2 ] /= 255.0f;

        glClear( GL_COLOR_BUFFER_BIT );

        glUseProgram( shaders );


        glm::mat4 transformMatrix(1.0f);
        transformMatrix = glm::translate( transformMatrix , glm::vec3( uXoffset , uXoffset , 0.0f ) );
        transformMatrix = glm::rotate( transformMatrix , counter * toRadians , glm::vec3( 3.0f , 5.0f , 2.0f) );
        transformMatrix = glm::scale(transformMatrix, glm::vec3(uXoffset, uXoffset, 1.0f));

        glUniformMatrix4fv( uniformModel , 1 , GL_FALSE , glm::value_ptr( transformMatrix ) );
        glUniform3fv( uniformColor , 1, glm::value_ptr(color));

        color[ 0 ] *= 255.0f;
        color[ 1 ] *= 255.0f;
        color[ 2 ] *= 255.0f;

        glBindVertexArray( vao );
        glDrawArrays( GL_TRIANGLES , 0 , 3 );
        glBindVertexArray( 0 );

        glUseProgram( 0 );

        glfwSwapBuffers( window );

    }

    glDeleteProgram( shaders );
    glfwTerminate();

    return 0;
}