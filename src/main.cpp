#include <iostream>
#include <fstream>
#include <vector>
#include <string>   

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>

#include "Mesh.h"
#include "Shaders.h"
#include "glWindow.h" 
#include "Camera.h"

std::vector<Mesh*> meshlist;
std::vector<Shaders> shaderlist;
glWindow mainWindow;
Camera camera;

void RenderMeshes( std::vector<Mesh*>& meshlist )
{
    for ( auto& mesh : meshlist )
        mesh->RenderMesh();
}

void ClearMeshes( std::vector<Mesh*>& meshlist )
{
    for ( auto& mesh : meshlist )
        mesh->ClearMesh();
}

void CreateTriangle() {

    float vertices[] {
       -0.5f,-0.5f, 0.0f, // Left
        0.5f,-0.5f, 0.0f, // Right
        0.0f, 0.5f, 0.0f, // Top
        0.0f, 0.0f, 1.0f  // Back Z
    };

    unsigned int indices[]{
        0, 1, 2, // Front
        3, 2, 1, // Right
        3, 2, 0, // Left
        3, 0, 1  // Bottom
    };

    for ( int i = 0; i < 4; i++ )
    {
        Mesh* obj = new Mesh();

        for ( int j = 0; j < 10; j += 3 )
            vertices[ j ]++;

        obj->CreateMesh( vertices , sizeof( vertices ) / sizeof( float ) , indices , sizeof( indices ) / sizeof( unsigned ) );
        meshlist.push_back( obj );
    }

}

int main()
{
    mainWindow.Initialize();

    /* Setup Dear ImGui context*/
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    /* Setup Platform/Renderer bindings*/
    ImGui_ImplGlfw_InitForOpenGL( mainWindow.GetWindow() , true );
    ImGui_ImplOpenGL3_Init( "#version 330" );
    /*Setup Dear ImGui style*/
    ImGui::StyleColorsDark();

    unsigned int uniformModel , uniformProjection, uniformView;
    shaderlist.push_back( Shaders() );

    CreateTriangle();
    shaderlist[ 0 ].CompileProgram();

    camera = Camera( glm::vec3( 0.f , 0.5f , 1.0f ) , glm::vec3( 0.f , 1.f , 0.f ) , -90.f , 0.f , 5.f , .5f );

    uniformModel = glGetUniformLocation( shaderlist[0].GetProgram() , "transformation" );
    uniformProjection = glGetUniformLocation( shaderlist[ 0 ].GetProgram() , "projection" );
    uniformView = glGetUniformLocation( shaderlist[ 0 ].GetProgram() , "view" );

    float rotation = 0.0f;
    float rotationVector[ 3 ]{ 1.0f, 0.0f , 0.0f };
    float translation[ 3 ]{ 0.0f, 0.0f , -1.0f };
    bool rotate = false;
    std::string toggle = "Start";

    while ( !mainWindow.ShouldClose() )
    {     
        /* Poll for and process events */
        glfwPollEvents();

        glClearColor( 0.15f , 0.17f , 0.20f , 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*Handle Control Callbacks*/
        camera.KeyControl( mainWindow.GetKeys() , ImGui::GetIO().DeltaTime );
        camera.MouseControl( mainWindow.GetDeltaX() , mainWindow.GetDeltaY() , mainWindow.GetButtons()[ GLFW_MOUSE_BUTTON_2 ] );

        /*feed inputs to dear imgui, start new frame*/
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        if ( rotate )
        {
            if ( rotation >= 360 )
                rotation = 0;

            rotation++;
            toggle = "Stop";
        }
        else
        {
            toggle = "Start";
        }

        /*Render GUI*/
        ImGui::Begin( (const char*)glGetString( GL_RENDERER ) , NULL, ImGuiWindowFlags_AlwaysAutoResize |  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
        ImGui::Text( "Delta Time: %.3f ms (%.1f FPS)" , 1000.0f * ImGui::GetIO().DeltaTime , ImGui::GetIO().Framerate );
        ImGui::NewLine();
        ImGui::SliderFloat3( "Translation" , translation , -10.0f , 10.0f , "%.2f" );
        ImGui::NewLine();
        if ( ImGui::Button( toggle.c_str() ) )
            rotate = !rotate;
        ImGui::SliderFloat( "Rotation" , &rotation , 0 , 360 );
        ImGui::SliderFloat3( "Rotation Vector" , rotationVector , -5.0f , 5.0f , "%.1f" );
        ImGui::End();

        /*Use Shaders*/
        shaderlist[ 0 ].UseShader();

        glm::mat4 projectionMatrix = glm::perspective( glm::radians( mainWindow.GetFov() ) , mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight() , 0.1f , 100.0f );
        glUniformMatrix4fv( uniformProjection , 1 , GL_FALSE , glm::value_ptr( projectionMatrix ) );
        glUniformMatrix4fv( uniformView , 1 , GL_FALSE , glm::value_ptr( camera.CalculateViewMatrix() ));

        /*Transformation*/
        glm::mat4 transformMatrix( 1.0f );
        transformMatrix = glm::translate( transformMatrix , glm::vec3( translation[ 0 ] , translation[ 1 ] , translation[ 2 ] ) );
        transformMatrix = glm::rotate( transformMatrix , glm::radians( rotation ) , glm::vec3( rotationVector[ 0 ] , rotationVector[ 1 ] , rotationVector[ 2 ] ) );
        glUniformMatrix4fv( uniformModel , 1 , GL_FALSE , glm::value_ptr( transformMatrix ) );

        RenderMeshes( meshlist );

        /*Draw GUI to screen*/
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

        /*Check for Errors*/
        GLenum err;
        while ( ( err = glGetError() ) != GL_NO_ERROR )
            std::cout << std::hex << "OpenGL Error: 0x" << err << std::endl;

        mainWindow.SwapBuffers();

    }

    ClearMeshes( meshlist );

    /*Clean Up IMGUI*/
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}