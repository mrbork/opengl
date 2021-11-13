#include "pch.h"

#include "Mesh.h"
#include "Shaders.h"
#include "glWindow.h" 
#include "Camera.h"
#include "Texture.h"
#include "Material.h"

std::vector<Mesh*> meshlist;
std::vector<Shaders> shaderlist;

glWindow mainWindow;
Camera camera;
Texture texture;
DirectionalLight mainLight;
PointLight pointLight[ MAX_POINTLIGHT_COUNT ];
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


    float floorVertices[ 32 ]
    {
        -10.0f, -2.0f, -10.0f,  0.0f,  0.0f, 0.0f, -1.0f, 0.0f,
         10.0f, -2.0f, -10.0f, 10.0f,  0.0f, 0.0f, -1.0f, 0.0f,
        -10.0f, -2.0f,  10.0f,  0.0f, 10.0f, 0.0f, -1.0f, 0.0f,
         10.0f, -2.0f,  10.0f, 10.0f, 10.0f, 0.0f, -1.0f, 0.0f
    };

    unsigned int floorIndices[ 6 ]
    {
        0, 2, 1,
        1, 2, 3
    };

    Mesh* obj2 = new Mesh();
    obj2->CreateMesh( floorVertices , 32 , floorIndices , 12 );
    meshlist.push_back( obj2 );
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

    shaderlist.push_back( Shaders() );

    CreateTriangle();
    shaderlist[ 0 ].CompileProgram();

    camera = Camera( glm::vec3( 0.f , 0.5f , 1.0f ) , glm::vec3( 0.f , 1.f , 0.f ) , -90.f , 0.f , 5.f , .5f );

    texture = Texture( "Textures/white.jpg" );
    texture.Load();

    mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 0.2f , 1.0f, 0.0f, 1.0f, 1.0f);
    pointLight[ 0 ] = PointLight( 0.0f , 1.0f , 0.0f , 0.1f , 1.0f , 0.5f , 0.0f , 0.0f , 0.3f , 0.2f , 0.1f );
    pointLight[ 1 ] = PointLight( 1.0f , 0.0f , 0.0f , 0.1f , 1.0f , -2.0f , 0.0f , 0.0f , 0.3f , 0.2f , 0.1f );

    unsigned int pointLightCount = 2;

    material = Material( 1.0f , 64.0f );

    float rotation = 0.0f;
    glm::vec3 rotationVector{ 0.0f, 1.0f , 0.0f };
    glm::vec3 translation{ 0.0f, 0.0f , -1.0f };

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

        ImGui::ShowDemoWindow();

        {
            /*Render GUI*/
            ImGui::Begin( ( const char* )glGetString( GL_RENDERER ) , NULL , ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse );
            ImGui::Text( "Delta Time: %.3f ms (%.1f FPS)" , 1000.0f * ImGui::GetIO().DeltaTime , ImGui::GetIO().Framerate );
            ImGui::NewLine();
            ImGui::Text( "Pitch: %.1f" , camera.GetViewAnglesY() );
            ImGui::Text( "Yaw: %.1f" , camera.GetViewAnglesX() );
            ImGui::NewLine();
            ImGui::Text( "Transformations" );
            ImGui::DragFloat3( "Translation" , &translation.x , 0.1f );
            ImGui::SliderFloat( "Rotation" , &rotation , 0 , 360 );
            ImGui::SameLine();
            if ( ImGui::Button( toggle.c_str() ) )
                rotate = !rotate;
            ImGui::SliderFloat3( "Rotation Vector" , &rotationVector.x , -5.0f , 5.0f , "%.1f" );

            for ( int i = 0; i < 3; i++ )
            {
                ImGui::PushID( i );
                bool node = ImGui::TreeNode( "Pointlight" , "%d", i);

                if ( node )
                {
                    ImGui::Text( "Test" );
                    ImGui::TreePop();
                }
                ImGui::PopID();
            }

            ImGui::End();
        }

        /*Use Shaders*/
        shaderlist[ 0 ].UseShader();
        //Use Texture1
        texture.Use();
        material.UseMaterial( shaderlist[ 0 ].GetSpecularIntensity(), shaderlist[ 0 ].GetShininess() );

        /*Use lighting*/
        shaderlist[ 0 ].SetDirectionalLight( &mainLight );
        shaderlist[ 0 ].SetPointLight( pointLight , pointLightCount );

        /*Setup Camera*/
        glm::mat4 projectionMatrix = glm::perspective( glm::radians( 70.0f ) , mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight() , 0.1f , 100.0f );
        glUniformMatrix4fv( shaderlist[ 0 ].GetProjection() , 1 , GL_FALSE , glm::value_ptr( projectionMatrix ) );
        glUniformMatrix4fv( shaderlist[ 0 ].GetView() , 1 , GL_FALSE , glm::value_ptr( camera.CalculateViewMatrix() ));
        glUniform3f( shaderlist[ 0 ].GetCameraPosition() , camera.GetCameraPosition().x , camera.GetCameraPosition().y , camera.GetCameraPosition().z );

        /*Transformation*/
        glm::mat4 transformMatrix( 1.0f );
        transformMatrix = glm::translate( transformMatrix , translation );
        transformMatrix = glm::rotate( transformMatrix , glm::radians( rotation ) , rotationVector );
        //transformMatrix = glm::scale( transformMatrix , glm::vec3( 1.0f , 1.0f , 1.0f ) );
        glUniformMatrix4fv( shaderlist[ 0 ].GetModel() , 1 , GL_FALSE , glm::value_ptr( transformMatrix ) );

        meshlist[ 0 ]->RenderMesh();
        meshlist[ 1 ]->RenderMesh();

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