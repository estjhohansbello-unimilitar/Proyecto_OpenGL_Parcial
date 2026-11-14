
#ifndef CUBE_CLASS_H
#define CUBE_CLASS_H

#include "Model.h"
#include "ModelLoader.h"
#include "Cube.h"
#include <glad/glad.h>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class Cube
{
public:
    VAO vao;
    VBO* vbo;
    EBO* ebo;

    Cube();
    ~Cube();

    void Draw();
};

#endif
