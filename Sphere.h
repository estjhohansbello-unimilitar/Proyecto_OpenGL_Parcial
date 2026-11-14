#ifndef SPHERE_CLASS_H
#define SPHERE_CLASS_H

#include <glad/glad.h>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class Sphere
{
public:
    VAO vao;
    VBO* vbo;
    EBO* ebo;

    GLsizei indexCount;

    Sphere();
    ~Sphere();

    void Draw();
};

#endif