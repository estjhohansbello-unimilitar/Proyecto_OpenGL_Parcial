
#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <vector>
#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

// Información de cada vértice
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
	glm::vec2 texCoords;// coordenadas UV
};

class Mesh
{
public:

    VAO vao;

	std::unique_ptr<VBO> vbo; // Puntero único al objeto VBO, que almacena los datos de los vértices en la GPU
	std::unique_ptr<EBO> ebo; // Puntero único al objeto EBO, que almacena los índices de los vértices en la GPU

	std::vector<Vertex> vertices; // vector de vértices
	std::vector<GLuint> indices; // vector de índices que definen cómo se conectan los vértices para formar triángulos
    
	int materialIndex = -1; // Índice del material asociado a este mesh, -1 indica que no hay material asignado

    Mesh(
		const std::vector<Vertex>& vertices, // Constructor que recibe un vector de vértices y un vector de índices
		const std::vector<GLuint>& indices // Inicializa el mesh con los vértices y los índices proporcionados
    );

    ~Mesh();

    void Draw();

    void Delete();
};

#endif

