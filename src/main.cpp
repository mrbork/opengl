#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Vertex Shader
static const std::string vShader =
"#version 330 core                      \n"
"                                       \n"
"layout(location = 0) in vec4 position; \n"
"                                       \n"
"void main()                            \n"
"{                                      \n"
"   gl_Position = position;             \n"
"}                                      \n";

//Fragment Shader
static const std::string fShader =
"#version 330 core                      \n"
"                                       \n"
"layout(location = 0) out vec4 color;   \n"
"                                       \n"
"void main()                            \n"
"{                                      \n"
"   color = vec4(1.0f,0.0f,0.0f,1.0f);  \n"
"}                                      \n";

unsigned int vao , vbo , shaders;

void CreateTriangle() {

    float vertices[] {
        -1.0f,-1.0f,
         1.0f,-1.0f,
         0.0f, 1.0f
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

void AddShader(unsigned int program, unsigned int type, const std::string src) {

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
        printf( "Failed to compile the %d shader: %s\n" , type, message );
        return;
    }

    glAttachShader( shaders , shader );
}

void CompileShaders() {
    shaders = glCreateProgram();

    if ( !shaders )
        printf( "Failed to create shader program.\n" );

    AddShader( shaders , GL_VERTEX_SHADER , vShader );
    AddShader( shaders , GL_FRAGMENT_SHADER , fShader );

    glLinkProgram( shaders );

    int result;
    char message[1024];

    glGetProgramiv( shaders , GL_LINK_STATUS , &result );
    if ( !result ) 
    {
        glGetProgramInfoLog( shaders , 1024 , NULL , message );
        printf( "Failed to link program: %s\n" , message );
        return;
    }

    glValidateProgram( shaders );
    glGetProgramiv( shaders , GL_VALIDATE_STATUS , &result );
    if ( !result )
    {
        glGetProgramInfoLog( shaders , 1024 , NULL , message );
        printf( "Failed to validate program: %s\n" , message );
        return;
    }

}


int main()
{

    /* Initialize the library */
    if ( !glfwInit() ) {
        printf( "Failed to initialize GLFW." );
        glfwTerminate();
        return 1;
    }

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);
    if ( !window )
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if ( glewInit() != GLEW_OK )
        printf("Failed to initialize GLEW\n");

    printf( "%s" , glGetString( GL_VERSION ) );

    CreateTriangle();
    CompileShaders();

    /* Loop until the user closes the window */
    while ( !glfwWindowShouldClose(window) )
    {
        /* Poll for and process events */
        glfwPollEvents();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram( shaders );

        glBindVertexArray( vao );
        glDrawArrays( GL_TRIANGLES , 0 , 3 );
        glBindVertexArray( 0 );

        glUseProgram( 0 );

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

    }

    glDeleteProgram( shaders );

    return 0;
}