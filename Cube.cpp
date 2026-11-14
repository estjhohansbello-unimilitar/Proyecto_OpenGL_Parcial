
#include "Cube.h"

#include <cstddef>

// Estructura de cada vertice
struct CubeVertex
{
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat texCoords[2];
};

Cube::Cube()
{
    // Vertices del cubo: posicion, normal y coordenadas UV
    CubeVertex vertices[] =
    {
        // Cara frontal
        {{-0.5f,-0.5f, 0.5f},{ 0, 0, 1},{0,0}},
        {{ 0.5f,-0.5f, 0.5f},{ 0, 0, 1},{1,0}},
        {{ 0.5f, 0.5f, 0.5f},{ 0, 0, 1},{1,1}},
        {{-0.5f, 0.5f, 0.5f},{ 0, 0, 1},{0,1}},

        // Cara trasera
        {{ 0.5f,-0.5f,-0.5f},{ 0, 0,-1},{0,0}},
        {{-0.5f,-0.5f,-0.5f},{ 0, 0,-1},{1,0}},
        {{-0.5f, 0.5f,-0.5f},{ 0, 0,-1},{1,1}},
        {{ 0.5f, 0.5f,-0.5f},{ 0, 0,-1},{0,1}},

        // Cara izquierda
        {{-0.5f,-0.5f,-0.5f},{-1, 0, 0},{0,0}},
        {{-0.5f,-0.5f, 0.5f},{-1, 0, 0},{1,0}},
        {{-0.5f, 0.5f, 0.5f},{-1, 0, 0},{1,1}},
        {{-0.5f, 0.5f,-0.5f},{-1, 0, 0},{0,1}},

        // Cara derecha
        {{ 0.5f,-0.5f, 0.5f},{ 1, 0, 0},{0,0}},
        {{ 0.5f,-0.5f,-0.5f},{ 1, 0, 0},{1,0}},
        {{ 0.5f, 0.5f,-0.5f},{ 1, 0, 0},{1,1}},
        {{ 0.5f, 0.5f, 0.5f},{ 1, 0, 0},{0,1}},

        // Cara superior
        {{-0.5f, 0.5f, 0.5f},{ 0, 1, 0},{0,0}},
        {{ 0.5f, 0.5f, 0.5f},{ 0, 1, 0},{1,0}},
        {{ 0.5f, 0.5f,-0.5f},{ 0, 1, 0},{1,1}},
        {{-0.5f, 0.5f,-0.5f},{ 0, 1, 0},{0,1}},

        // Cara inferior
        {{-0.5f,-0.5f,-0.5f},{ 0,-1, 0},{0,0}},
        {{ 0.5f,-0.5f,-0.5f},{ 0,-1, 0},{1,0}},
        {{ 0.5f,-0.5f, 0.5f},{ 0,-1, 0},{1,1}},
        {{-0.5f,-0.5f, 0.5f},{ 0,-1, 0},{0,1}}
    };

    GLuint indices[] =
    {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        8,9,10, 10,11,8,
        12,13,14, 14,15,12,
        16,17,18, 18,19,16,
        20,21,22, 22,23,20
    };

    // Crear VAO, VBO y EBO
    vao.Bind();

    vbo = new VBO(
        reinterpret_cast<GLfloat*>(vertices),
        sizeof(vertices)
    );

    ebo = new EBO(indices, sizeof(indices));

    // Posicion
    vao.LinkAttrib(
        *vbo, 0, 3, GL_FLOAT,
        sizeof(CubeVertex),
        (void*)offsetof(CubeVertex, position)
    );

    // Normal
    vao.LinkAttrib(
        *vbo, 1, 3, GL_FLOAT,
        sizeof(CubeVertex),
        (void*)offsetof(CubeVertex, normal)
    );

    // Coordenadas de textura
    vao.LinkAttrib(
        *vbo, 2, 2, GL_FLOAT,
        sizeof(CubeVertex),
        (void*)offsetof(CubeVertex, texCoords)
    );

    vao.Unbind();
    vbo->Unbind();
    ebo->Unbind();
}

Cube::~Cube()
{
    vao.Delete();

    vbo->Delete();
    ebo->Delete();

    delete vbo;
    delete ebo;
}

void Cube::Draw()
{
    vao.Bind();

    glDrawElements(
        GL_TRIANGLES,
        36,
        GL_UNSIGNED_INT,
        0
    );

    vao.Unbind();
}
