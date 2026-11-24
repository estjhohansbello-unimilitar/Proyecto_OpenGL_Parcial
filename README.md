#  Proyecto OpenGL Parcial — Escena Invernal 3D

**Autor:** Jhohan's Mateo Bello Jaimes  
**Código:** `6014400`

Proyecto académico desarrollado en **C++** y **OpenGL** enfocado en la construcción y visualización de una escena tridimensional interactiva aplicando distintas técnicas de computación gráfica. La escena recrea un entorno invernal con un castillo, dragones, guerreros, una luna y un dragón volador animado.

---

##  Características principales

- Carga de modelos tridimensionales en formato **glTF**.
- Gestión de geometrías, materiales y texturas.
- Transformaciones geométricas mediante matrices (traslación, rotación y escalado).
- Control interactivo de cámara.
- Iluminación local mediante el **modelo Phong**.
- Generación de sombras en tiempo real con **Shadow Mapping**.
- Animación de trayectorias mediante **curvas cúbicas de Bézier**.

---

##  Tecnologías y bibliotecas

| Tecnología / Biblioteca | Propósito en el proyecto |
| :--- | :--- |
| **C++** | Lenguaje principal de desarrollo y arquitectura modular. |
| **OpenGL** | API de renderizado gráfico tridimensional. |
| **GLFW** | Creación y gestión de ventanas, contexto y eventos de entrada. |
| **GLAD** | Carga dinámica de punteros de funciones de OpenGL. |
| **GLM** | Álgebra lineal: vectores, matrices y transformaciones geométricas. |
| **TinyGLTF** | Carga y parseo de modelos 3D en formato `.gltf`. |
| **stb_image** | Carga de imágenes para texturas y mapas. |
| **GLSL** | Lenguaje de sombreado para *vertex* y *fragment shaders*. |

---

##  Arquitectura del software

El código está organizado bajo una arquitectura modular orientada a objetos en C++, separando la coordinación general, la carga de recursos, la gestión de geometría, el control de cámara y el *pipeline* de renderizado.

### Estructura del proyecto

```text
Proyecto_OpenGL_Parcial/
├── Archivos de encabezado/
│   ├── Camera.h
│   ├── Cube.h
│   ├── EBO.h
│   ├── Mesh.h
│   ├── Model.h
│   ├── ModelLoader.h
│   ├── shaderClass.h
│   ├── Sphere.h
│   ├── stb_image.h
│   ├── Texture.h
│   ├── VAO.h
│   └── VBO.h
├── Archivos de origen/
│   ├── Camera.cpp
│   ├── Cube.cpp
│   ├── EBO.cpp
│   ├── glad.c
│   ├── Main.cpp
│   ├── Mesh.cpp
│   ├── Model.cpp
│   ├── ModelLoader.cpp
│   ├── shaderClass.cpp
│   ├── Sphere.cpp
│   ├── stb_image.cpp
│   ├── Texture.cpp
│   ├── VAO.cpp
│   └── VBO.cpp
└── README.md
