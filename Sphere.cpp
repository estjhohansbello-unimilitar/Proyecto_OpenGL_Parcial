#include "Sphere.h"

#include <vector>
#include <cmath>
#include <cstddef>

// Estructura de cada vertice
struct SphereVertex
{
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat texCoords[2];
};

Sphere::Sphere()
{
    const unsigned int sectors = 36;
    const unsigned int stacks = 18;

    const float PI = 3.14159265359f;

    std::vector<SphereVertex> vertices;
    std::vector<GLuint> indices;

    // -----------------------------------------------------------
    // GENERAR VERTICES DE LA ESFERA
    // -----------------------------------------------------------

    for (unsigned int i = 0; i <= stacks; i++)
    {
        float stackAngle = PI / 2.0f -
            PI * (float)i / (float)stacks;

        float xy = cosf(stackAngle);
        float y = sinf(stackAngle);

        for (unsigned int j = 0; j <= sectors; j++)
        {
            float sectorAngle =
                2.0f * PI * (float)j / (float)sectors;

            float x = xy * cosf(sectorAngle);
            float z = xy * sinf(sectorAngle);

            SphereVertex vertex;

            // Posicion
            vertex.position[0] = x;
            vertex.position[1] = y;
            vertex.position[2] = z;

            // Normal: en una esfera unitaria coincide
            // con la posicion del vertice.
            vertex.normal[0] = x;
            vertex.normal[1] = y;
            vertex.normal[2] = z;

            // Coordenadas UV
            vertex.texCoords[0] =
                (float)j / (float)sectors;

            vertex.texCoords[1] =
                (float)i / (float)stacks;

            vertices.push_back(vertex);
        }
    }

    // -----------------------------------------------------------
    // GENERAR INDICES
    // -----------------------------------------------------------

    for (unsigned int i = 0; i < stacks; i++)
    {
        unsigned int k1 = i * (sectors + 1);
        unsigned int k2 = k1 + sectors + 1;

        for (unsigned int j = 0; j < sectors; j++, k1++, k2++)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stacks - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    indexCount = (GLsizei)indices.size();

    // -----------------------------------------------------------
    // CREAR VAO, VBO Y EBO
    // -----------------------------------------------------------

    vao.Bind();

    vbo = new VBO(
        reinterpret_cast<GLfloat*>(vertices.data()),
        (GLsizeiptr)(vertices.size() * sizeof(SphereVertex))
    );

    ebo = new EBO(
        indices.data(),
        (GLsizeiptr)(indices.size() * sizeof(GLuint))
    );

    // Posicion
    vao.LinkAttrib(
        *vbo, 0, 3, GL_FLOAT,
        sizeof(SphereVertex),
        (void*)offsetof(SphereVertex, position)
    );

    // Normal
    vao.LinkAttrib(
        *vbo, 1, 3, GL_FLOAT,
        sizeof(SphereVertex),
        (void*)offsetof(SphereVertex, normal)
    );

    // Coordenadas de textura
    vao.LinkAttrib(
        *vbo, 2, 2, GL_FLOAT,
        sizeof(SphereVertex),
        (void*)offsetof(SphereVertex, texCoords)
    );

    vao.Unbind();
    vbo->Unbind();
    ebo->Unbind();
}

Sphere::~Sphere()
{
    vao.Delete();

    vbo->Delete();
    ebo->Delete();

    delete vbo;
    delete ebo;
}

void Sphere::Draw()
{
    vao.Bind();

    glDrawElements(
        GL_TRIANGLES,
        indexCount,
        GL_UNSIGNED_INT,
        0
    );

    vao.Unbind();
}