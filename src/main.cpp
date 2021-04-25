#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

static unsigned int CompileShader( unsigned int type , const std::string& source ) {

    unsigned int shader = glCreateShader( type );
    const char* src = source.c_str( );
    glShaderSource( shader , 1 , &src , nullptr );
    glCompileShader( shader );

    int compileResult;
    glGetShaderiv( shader , GL_COMPILE_STATUS , &compileResult );
    if ( compileResult == GL_FALSE ) {

        int length;
        glGetShaderiv( shader , GL_INFO_LOG_LENGTH , &length );
        char* message = ( char* ) alloca( length * sizeof( char ) );
        glGetShaderInfoLog( shader , length , &length , message );
        std::cout << message << std::endl;

        glDeleteShader( shader );
        return 0;
    }

    return shader;
}

static unsigned int CreateShader( const std::string& vertexShader , const std::string& fragmentShader ) {

    unsigned int program = glCreateProgram( );
    unsigned int vShader = CompileShader( GL_VERTEX_SHADER , vertexShader );
    unsigned int fShader = CompileShader( GL_FRAGMENT_SHADER , fragmentShader );

    glAttachShader( program , vShader );
    glAttachShader( program , fShader );
    glLinkProgram( program );
    glValidateProgram( program );

    glDetachShader( program, vShader );
    glDetachShader( program,  fShader );

    return program;
}

int main(void)
{

    /* Initialize the library */
    if ( !glfwInit( ) ) {
        printf( "Failed to initialize GLFW." );
        glfwTerminate( );
        return 1;
    }

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if ( !window )
    {
        glfwTerminate( );
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;

    if ( glewInit() != GLEW_OK )
        printf("Failed to initialize GLEW\n");

    printf( "%s" , glGetString( GL_VERSION ) );

    unsigned int triangleBuffer;
    float pos[ 6 ] {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.0f, 0.5f
    };
    glGenBuffers( 1 , &triangleBuffer );
    glBindBuffer( GL_ARRAY_BUFFER , triangleBuffer );
    glBufferData( GL_ARRAY_BUFFER , 6 * sizeof( float ) , &pos , GL_STATIC_DRAW);
    glVertexAttribPointer( 0 , 2 , GL_FLOAT , GL_FALSE , sizeof( float ) * 2 , 0 );
    glEnableVertexAttribArray( 0 );
    
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shader = CreateShader( vertexShader , fragmentShader );
    glUseProgram( shader );

    /* Loop until the user closes the window */
    while ( !glfwWindowShouldClose(window) )
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays( GL_TRIANGLES , 0 , 3 );

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram( shader );
    glfwTerminate();
    return 0;
}