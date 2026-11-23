Jhohan´s Mateo Bello Jaimes  6014400
Proyecto OpenGL Parcial

 Descripción del proyecto
Proyecto académico desarrollado en C++ utilizando OpenGL, 
cuyo objetivo es construir y representar una escena tridimensional mediante diferentes técnicas de computación gráfica.
La escena está compuesta por un entorno de invierno, un castillo, dragones, guerreros, una luna y un dragón volador.

El proyecto integra funcionalidades como:
- Carga de modelos tridimensionales mediante archivos glTF.
- Gestión de geometrías, materiales y texturas.
- Transformaciones geométricas mediante matrices.
- Control de cámara.
- Iluminación mediante el modelo Phong.
- Generación de sombras mediante Shadow Mapping.
- Animación de objetos mediante curvas de Bézier.

2. Tecnologías y bibliotecas
Tecnología            Utilización
C++          Lenguaje de programación principal.

OpenGL       Representación gráfica tridimensional.

GLFW         Creación y administración de ventanas y eventos.

GLAD         Carga de funciones de OpenGL.

GLM          Operaciones matemáticas, vectores y matrices.

TinyGLTF     Lectura de modelos tridimensionales en formato glTF.

stb_image    Carga de imágenes utilizadas como texturas.

GLSL          Lenguaje utilizado para desarrollar los shaders.



3. Arquitectura del software

El proyecto utiliza una arquitectura modular basada en clases de C++, 
donde cada componente tiene una responsabilidad específica dentro de la aplicación.

La arquitectura separa la inicialización y coordinación de la escena, 
la carga de modelos, la administración de geometrías, el control de cámara y el procesamiento gráfico.

--  Estructura general del proyecto --

La organización de los archivos de código fuente es la siguiente:

Proyecto_OpenGL_Parcial/
│
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
│
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
│
└── README.md

Nota: los modelos, las texturas, los shaders y las demás dependencias se encuentran organizados en sus respectivas ubicaciones dentro del proyecto.

---  Descripción de los componentes -------

A. Módulo principal

Archivos: Main.cpp
Es el punto de entrada y coordinación de la aplicación.
Sus principales responsabilidades son:
- Inicializar GLFW, GLAD y el contexto de OpenGL.
- Crear y configurar la ventana.
- Cargar los diferentes modelos glTF.
- Crear las instancias de los modelos.
- Configurar las transformaciones geométricas.
- Configurar la cámara y los shaders.
- Ejecutar el ciclo principal de renderizado.
- Coordinar las pasadas necesarias para generar y aplicar sombras.
- Liberar los recursos gráficos al finalizar la ejecución.

B. Módulo de cámara
Archivos: Camera.h y Camera.cpp
- Se encarga de controlar la cámara utilizada para visualizar el entorno tridimensional.
- Permite gestionar la posición y orientación de la cámara, así como su movimiento mediante los dispositivos de entrada.

C. Módulo de carga de modelos
Archivos: ModelLoader.h y ModelLoader.cpp
- Se encarga de leer los archivos glTF mediante la biblioteca TinyGLTF.
- Obtiene la información necesaria de cada modelo, incluyendo sus mallas, materiales, texturas y estructura de nodos.
- La función LoadGLTF permite cargar los archivos y almacenar su información en una estructura tinygltf::Model.

D. Módulo de modelos
Archivos: Model.h y Model.cpp
- Administra la información de los modelos tridimensionales que se incorporan a la escena.
- Entre sus principales responsabilidades se encuentran:
- Cargar y organizar las mallas.
- Gestionar los materiales.
- Calcular los límites espaciales de los modelos.
- Procesar las transformaciones de los nodos glTF.
- Dibujar los modelos en la escena.
- Dibujar los modelos desde la perspectiva de la luz para generar el mapa de profundidad.

Entre sus funciones principales se encuentran LoadMaterials, LoadMesh, CalculateBounds, Draw y DrawDepth.
También utiliza la estructura MaterialData para almacenar información de los materiales, como el color base, la textura, 
el umbral de transparencia y la configuración de iluminación.

E. Módulo de geometría
Archivos: Mesh.h, Mesh.cpp, VAO.h, VAO.cpp, VBO.h, VBO.cpp, EBO.h y EBO.cpp.
- Este módulo administra la información geométrica necesaria para representar los modelos.
- La clase Mesh organiza los vértices y los índices de cada malla.
Cada vértice contiene:
- Posición tridimensional.
- Vector normal.
- Coordenadas de textura UV.

Para enviar esta información a la GPU se utilizan tres estructuras de OpenGL:
- VAO (Vertex Array Object): almacena la configuración de los atributos de los vértices.
- VBO (Vertex Buffer Object): almacena los datos de los vértices.
- EBO (Element Buffer Object): almacena los índices que determinan cómo se conectan los vértices.

F. Módulo de texturas
Archivos: Texture.h, Texture.cpp, stb_image.h y stb_image.cpp.
- Se encarga de gestionar la carga y utilización de imágenes como texturas dentro de los modelos tridimensionales.
- Se utiliza stb_image como biblioteca auxiliar para leer los archivos de imagen.

G. Módulo de shaders
Archivos: shaderClass.h y shaderClass.cpp.
- Administra la creación, compilación y utilización de los programas de shaders de OpenGL.
- Los shaders permiten realizar el procesamiento de los vértices y calcular el resultado visual de los fragmentos de la escena.
Se utilizan dos programas principales:
- model.vert y model.frag: procesamiento de modelos, texturas e iluminación.
- shadow.vert y shadow.frag: procesamiento utilizado durante la generación del mapa de profundidad.

H. Módulo de geometrías auxiliares
Archivos: Cube.h, Cube.cpp, Sphere.h y Sphere.cpp.
- Contiene clases para representar geometrías básicas, como cubos y esferas, utilizadas durante las pruebas y el desarrollo de funcionalidades gráficas.

4. Técnicas de computación gráfica implementadas
4.1. Carga de modelos glTF
- Se utiliza TinyGLTF para cargar los diferentes modelos tridimensionales.
- Cada modelo se carga mediante su ruta correspondiente y posteriormente se crea una instancia de la clase Model, 
  encargada de organizar sus mallas y materiales.

4.2. Transformaciones geométricas
Se utiliza GLM para realizar transformaciones mediante matrices.
Las operaciones principales son:
- Traslación: modifica la posición de los objetos en los ejes X, Y y Z.
- Rotación: modifica la orientación de los modelos.
- Escalado: modifica las dimensiones de los objetos.
Estas operaciones permiten adaptar los modelos originales a las proporciones y distribución del escenario.

4.3. Animación mediante curvas de Bézier
Se implementa una curva cúbica de Bézier para controlar el movimiento del dragón volador.
La trayectoria se define mediante cuatro puntos de control y un parámetro que determina la posición del objeto sobre la curva.

El parámetro se actualiza utilizando el tiempo transcurrido entre fotogramas, 
permitiendo que el movimiento no dependa directamente de la velocidad de renderizado.

4.4. Iluminación Phong
Se implementa el modelo de iluminación Phong mediante tres componentes:
- Ambiental: proporciona iluminación general.
- Difusa: calcula la iluminación que recibe una superficie según la dirección de la luz.
- Especular: genera reflejos y brillos sobre las superficies.
Los cálculos se realizan mediante los shaders correspondientes.

4.5. Shadow Mapping
Se implementa la técnica Shadow Mapping para generar sombras dentro del escenario.
El procedimiento se divide en dos pasadas:
- Primera pasada: generación del mapa de profundidad.
Se utiliza un framebuffer asociado a una textura de profundidad de resolución 2048 × 2048.
La escena se representa desde la perspectiva de la fuente de luz y se almacena la información de profundidad de los objetos.

- Segunda pasada: renderizado de la escena.
Se vuelve al framebuffer principal y se dibujan los modelos utilizando sus materiales y la iluminación correspondiente.
El shader utiliza la matriz de espacio de luz y el mapa de profundidad para determinar qué fragmentos se encuentran en sombra.

5. Modelos que componen la escena
- Los modelos tridimensionales se cargan de manera independiente y posteriormente se integran mediante transformaciones geométricas.

Modelo                            Ruta

Entorno de invierno               Models/Environments/LandscapeWinter/scene.gltf

Castillo                          Models/Castle/scene.gltf
  
Dragones                          Models/Dragons/scene.gltf

Guerreros                         Models/Warriors/scene.gltf

Luna                              Models/Moon/scene.gltf

Dragón volador                    Models/FlyingDragon/scene.gltf

Flujo general de funcionamiento
- El funcionamiento de la aplicación sigue las siguientes etapas:
- Inicialización de GLFW y GLAD.
- Creación de la ventana y configuración del contexto OpenGL.
- Carga de los modelos tridimensionales mediante TinyGLTF.
- Creación de las instancias de los modelos.
- Configuración de la cámara y los shaders.
- Cálculo de las transformaciones geométricas.
- Actualización de las animaciones y movimientos de los objetos.
- Primera pasada de renderizado para generar el mapa de profundidad.
- Segunda pasada de renderizado para representar la escena con iluminación y sombras.
- Actualización de la ventana y procesamiento de eventos.
- Liberación de recursos y finalización de la aplicación.

Organización de los archivos
El proyecto se encuentra organizado en Visual Studio mediante dos grupos principales:
  - Archivos de encabezado (.h):
Contienen las declaraciones de las clases, estructuras, atributos y funciones.
  - Archivos de origen (.cpp):
Contienen la implementación de las funciones y la lógica de cada componente.
Esta separación facilita la organización, el mantenimiento y la comprensión del código.


