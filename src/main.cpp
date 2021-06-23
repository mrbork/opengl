#include "pch.h"

#include "Mesh.h"
#include "Shaders.h"
#include "glWindow.h" 
#include "Camera.h"
#include "Texture.h"
#include "Lighting.h"
#include "Material.h"

std::vector<Mesh*> meshlist;
std::vector<Shaders> shaderlist;

glWindow mainWindow;
Camera camera;
Texture texture;
Lighting lighting;
Material material;

void CalcAverageNormals( unsigned int* indices , unsigned int indicesCount , float* vertices , unsigned int verticesCount , unsigned int vertexLength , unsigned int normalOffset )
{
    for ( unsigned int i = 0; i < indicesCount; i += 3 )
    {
        unsigned int v1 = indices[i] * vertexLength;
        unsigned int v2 = indices[ i + 1 ] * vertexLength;
        unsigned int v3 = indices[ i + 2 ] * vertexLength;

        glm::vec3 line1( vertices[ v2 ] - vertices[ v1 ] , vertices[ v2 + 1 ] - vertices[ v1 + 1 ] , vertices[ v2 + 2 ] - vertices[ v1 + 2 ] );
        glm::vec3 line2( vertices[ v3 ] - vertices[ v1 ] , vertices[ v3 + 1 ] - vertices[ v1 + 1 ] , vertices[ v3 + 2 ] - vertices[ v1 + 2 ] );
        glm::vec3 normal = glm::normalize( glm::cross( line1 , line2 ) );

        v1 += normalOffset;
        v2 += normalOffset;
        v3 += normalOffset;

        vertices[ v1 ] += normal.x; vertices[ v1 + 1 ] += normal.y; vertices[ v1 + 2 ] += normal.z;
        vertices[ v2 ] += normal.x; vertices[ v2 + 1 ] += normal.y; vertices[ v2 + 2 ] += normal.z;
        vertices[ v3 ] += normal.x; vertices[ v3 + 1 ] += normal.y; vertices[ v3 + 2 ] += normal.z;
    }

    for ( unsigned int j = 0; j < verticesCount / vertexLength; j++ )
    {
        unsigned int offset = j * vertexLength + normalOffset;
        glm::vec3 normalVertex( vertices[ offset ] , vertices[ offset + 1 ] , vertices[ offset + 2 ] );
        normalVertex = glm::normalize( normalVertex );

        vertices[ offset ] = normalVertex.x;
        vertices[ offset + 1 ] = normalVertex.y;
        vertices[ offset + 2 ] = normalVertex.z;
    }
}

void CreateTriangle() {

    float vertices[64]{
       -0.5f, -0.5f, -0.3f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f, // Left
        0.5f, -0.5f, -0.3f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f, // Right
        0.0f,  0.5f,  0.0f,  0.5f, 1.0f,  0.0f, 0.0f, 0.0f, // Top
        0.0f, -0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f, // Back Z

        -0.5f, -0.5f, -0.3f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f, // Left
        0.5f, -0.5f, -0.3f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f, // Right
        0.0f,  0.5f,  0.0f,  0.5f, 1.0f,  0.0f, 0.0f, 0.0f, // Top
        0.0f, -0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f  // Back Z
    };

    unsigned int indices[24]{
        0, 1, 2, // Front
        3, 1, 2, // Right
        3, 0, 2, // Left
        3, 1, 0  // Bottom
    };

    for ( int i = 1; i < 2; i++ )
    {
        vertices[ 0 + 32 * i ]++;
        vertices[ 8 + 32 * i ]++;
        vertices[ 16 + 32 * i ]++;
        vertices[ 24 + 32 * i ]++;
    }

    for ( int j = 0; j < 12; j ++ )
    {
        indices[ j + 12 ] = indices[ j ] + 4;
    }

    CalcAverageNormals( indices , 24 , vertices , 64 , 8 , 5 );

    Mesh* obj = new Mesh();
    obj->CreateMesh( vertices , 64 , indices , 24 );
    meshlist.push_back( obj );

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

    unsigned int uniformModel , uniformProjection, uniformView, uniformCameraPosition,
        uniformambientColor, uniformambientIntensity, uniformDirection, uniformDiffuseIntensity, uniformSpecularIntensity, uniformShininess;
    float intensity = 0.2f;
    shaderlist.push_back( Shaders() );

    CreateTriangle();
    shaderlist[ 0 ].CompileProgram();

    camera = Camera( glm::vec3( 0.f , 0.5f , 1.0f ) , glm::vec3( 0.f , 1.f , 0.f ) , -90.f , 0.f , 5.f , .5f );

    texture = Texture( "Textures/brick.png" );
    texture.Load();

    lighting = Lighting(1.0f, 1.0f, 1.0f, intensity , 0.0f, 1.0f, 1.0f, 1.0f);
    material = Material( 1.0f , 64.0f );

    uniformModel = glGetUniformLocation( shaderlist[0].GetProgram() , "transformation" );
    uniformProjection = glGetUniformLocation( shaderlist[ 0 ].GetProgram() , "projection" );
    uniformView = glGetUniformLocation( shaderlist[ 0 ].GetProgram() , "view" );
    uniformCameraPosition = glGetUniformLocation( shaderlist[ 0 ].GetProgram() , "cameraPos" );
    uniformambientColor = glGetUniformLocation( shaderlist[ 0 ].GetProgram() , "directionalLight.color" );
    uniformambientIntensity = glGetUniformLocation( shaderlist[ 0 ].GetProgram() , "directionalLight.ambientIntensity" );
    uniformDirection = glGetUniformLocation( shaderlist[ 0 ].GetProgram() , "directionalLight.direction" );
    uniformDiffuseIntensity = glGetUniformLocation( shaderlist[ 0 ].GetProgram() , "directionalLight.diffuseIntensity" );
    uniformSpecularIntensity = glGetUniformLocation( shaderlist[ 0 ].GetProgram() , "material.specularIntensity" );
    uniformShininess = glGetUniformLocation( shaderlist[ 0 ].GetProgram() , "material.Shininess" );


    float rotation = 0.0f;
    glm::vec3 rotationVector{ 0.0f, 1.0f , 0.0f };
    glm::vec3 translation{ 0.0f, 0.0f , -1.0f };
    glm::vec3 ambientColor{ 1.0f, 1.0f, 1.0f };
    glm::vec3 lightDirection{ 0.0f, 1.0f, 1.0f };
    bool rotate = false;
    std::string toggle = "Start";

    while ( !mainWindow.ShouldClose() )
    {
        /* Poll for and process events */
        glfwPollEvents();

        /*Clear Buffer*/
        glClearColor( 0.15f , 0.17f , 0.20f , 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        /*Handle Control Callbacks*/
        if ( !io.WantCaptureMouse )
            camera.KeyControl( mainWindow.GetKeys() , ImGui::GetIO().DeltaTime , mainWindow.GetScrollDelta() );
        camera.MouseControl( mainWindow.GetMouseDeltaX() , mainWindow.GetMouseDeltaY() , mainWindow.GetButtons()[ GLFW_MOUSE_BUTTON_2 ] );


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

        /*feed inputs to dear imgui, start new frame*/
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            /*Render GUI*/
            ImGui::Begin( ( const char* )glGetString( GL_RENDERER ) , NULL , ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse );
            ImGui::Text( "Delta Time: %.3f ms (%.1f FPS)" , 1000.0f * ImGui::GetIO().DeltaTime , ImGui::GetIO().Framerate );
            ImGui::NewLine();
            ImGui::Text( "Pitch: %.1f" , camera.GetViewAnglesY() );
            ImGui::Text( "Yaw: %.1f" , camera.GetViewAnglesX() );
            ImGui::NewLine();
            ImGui::Text( "Lighting" );
            ImGui::SliderFloat( "Ambient Intensity" , &intensity , 0.0f , 1.0f );
            ImGui::SliderFloat3( "Ambient Color" , &ambientColor.x , 0.0f , 1.0f , "%.1f" );
            ImGui::SliderFloat3( "Light Direction" , &lightDirection.x , -2.0f , 2.0f , "%.1f" );
            ImGui::NewLine();
            ImGui::Text( "Transformations" );
            ImGui::DragFloat3( "Translation" , &translation.x , 0.1f );
            ImGui::SliderFloat( "Rotation" , &rotation , 0 , 360 );
            ImGui::SameLine();
            if ( ImGui::Button( toggle.c_str() ) )
                rotate = !rotate;
            ImGui::SliderFloat3( "Rotation Vector" , &rotationVector.x , -5.0f , 5.0f , "%.1f" );
            ImGui::End();
        }

        /*Use Shaders*/
        shaderlist[ 0 ].UseShader();
        //Use Texture1
        texture.Use();
        material.UseMaterial( uniformSpecularIntensity , uniformShininess );
        lighting.Update( ambientColor , intensity , lightDirection );
        lighting.Use( uniformambientColor , uniformambientIntensity , uniformDirection, uniformDiffuseIntensity);

        glm::mat4 projectionMatrix = glm::perspective( glm::radians( 70.0f ) , mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight() , 0.1f , 100.0f );
        glUniformMatrix4fv( uniformProjection , 1 , GL_FALSE , glm::value_ptr( projectionMatrix ) );
        glUniformMatrix4fv( uniformView , 1 , GL_FALSE , glm::value_ptr( camera.CalculateViewMatrix() ));
        glUniform3f( uniformCameraPosition , camera.GetCameraPosition().x , camera.GetCameraPosition().y , camera.GetCameraPosition().z );

        /*Transformation*/
        glm::mat4 transformMatrix( 1.0f );
        transformMatrix = glm::translate( transformMatrix , translation );
        transformMatrix = glm::rotate( transformMatrix , glm::radians( rotation ) , rotationVector );
        //transformMatrix = glm::scale( transformMatrix , glm::vec3( 1.0f , 1.0f , 1.0f ) );
        glUniformMatrix4fv( uniformModel , 1 , GL_FALSE , glm::value_ptr( transformMatrix ) );

        meshlist[ 0 ]->RenderMesh();

        /*Check for Errors*/
        GLenum err;
        while ( ( err = glGetError() ) != GL_NO_ERROR )
            std::cout << std::hex << "OpenGL Error: 0x" << err << std::endl;

        /*Draw GUI to screen*/
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

        mainWindow.SwapBuffers();

    }

    meshlist[ 0 ]->ClearMesh();

    /*Clean Up IMGUI*/
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}