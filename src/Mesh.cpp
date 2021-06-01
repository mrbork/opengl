#include "pch.h"
#include "Mesh.h"

void Mesh::CreateMesh( float* vertices , unsigned int numVertices , unsigned int* indices , unsigned int numIndices )
{
    uIndices = numIndices;

    glGenVertexArrays( 1 , &vao );
    glBindVertexArray( vao );

    /*Create Buffers*/
    glGenBuffers( 1 , &ibo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , ibo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER , numIndices * sizeof(unsigned) , indices , GL_STATIC_DRAW );

    glGenBuffers( 1 , &vbo );
    glBindBuffer( GL_ARRAY_BUFFER , vbo );
    glBufferData( GL_ARRAY_BUFFER , numVertices * sizeof(float) , vertices , GL_STATIC_DRAW );

    glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , sizeof( vertices[ 0 ] ) * 8 , 0 );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 1 , 2 , GL_FLOAT , GL_FALSE , sizeof( vertices[ 0 ] ) * 8 , ( void* )( sizeof( vertices[ 0 ] ) * 3 ) );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 2 , 3 , GL_FLOAT , GL_FALSE , sizeof( vertices[ 0 ] ) * 8 , ( void* )( sizeof( vertices[ 0 ] ) * 5 ) );
    glEnableVertexAttribArray( 2 );

    /*Unbind Objects*/
    glBindBuffer( GL_ARRAY_BUFFER , 0 );

    glBindVertexArray( 0 );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , 0 );
}

void Mesh::RenderMesh() 
{
    glBindVertexArray( vao );

    /*Draw*/
    glDrawElements( GL_TRIANGLES , uIndices , GL_UNSIGNED_INT , 0 );

    glBindVertexArray( 0 );
}

void Mesh::ClearMesh() 
{
    if ( ibo != 0 ) {
        glDeleteBuffers( 1 , &ibo );
        ibo = 0;
    }
    if ( vbo != 0 ) {
        glDeleteBuffers( 1 , &vbo );
        vbo = 0;
    }
    if ( vao != 0 ) {
        glDeleteBuffers( 1 , &vao );
        vao = 0;
    }

    uIndices = 0;
}

Mesh::Mesh() 
{
	vao = 0;
	vbo = 0;
	ibo = 0;
    uIndices = 0;
}

Mesh::~Mesh()
{
    ClearMesh();
}
