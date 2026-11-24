#include <iostream> // std::cout, std::endl
#include <algorithm> // std::max
#include <memory>    // std::unique_ptr

#include <glad/glad.h> // para cargar las funciones de OpenGL
#include <GLFW/glfw3.h> // para crear la ventana y el contexto de OpenGL

#include <glm/glm.hpp> // permite trabajar con vectores y matrices
#include <glm/gtc/matrix_transform.hpp> // matrices de transformación (traslación, rotación, escalado, proyección)
#include <glm/gtc/type_ptr.hpp> // permite convertir matrices y vectores a punteros de tipo float

#include "shaderClass.h" // para compilar y enlazar shaders
#include "Camera.h" // para controlar la cámara con teclado y mouse

#include "Model.h" //para cargar y dibujar modelos glTF
#include "ModelLoader.h" // para cargar archivos glTF


//cubo de prueba
#include "Cube.h"

//esfera de prueba
#include "Sphere.h"

// ---------------------------------------------------------------
//RUTAS 
// ---------------------------------------------------------------

// RUTA DEL ESCENARIO
// (.glb, o .gltf con su .bin y texturas en la misma carpeta)
const char* SCENE_PATH = "Models/Environments/LandscapeWinter/scene.gltf";

// RUTA DEL CASTILLO
const char* CASTLE_PATH =
"Models/Castle/scene.gltf";

// Tamaño deseado del castillo
const float CASTLE_TARGET_SIZE = 0.29f;

// RUTA DE LA ESCENA DE DRAGONES
const char* DRAGONS_PATH = "Models/Dragons/scene.gltf";

// RUTA DE LA ESCENA DE GUERREROS
const char* WARRIORS_PATH = "Models/Warriors/scene.gltf";

// RUTA DE LA ESCENA DE LA LUNA
const char* MOON_PATH = "Models/Moon/scene.gltf";

//RUTA DEL DRAGON VOLADOR 
const char* FLYINGDRAGON_PATH = "Models/FlyingDragon/scene.gltf";

// Tamaño al que se escala el lado más largo del escenario, en unidades del mundo.
const float SCENE_TARGET_SIZE = 2.5f;

// ---------------------------------------------------------------
//FUNCION BEZIER
// ---------------------------------------------------------------
// Función para calcular un punto en una curva de Bezier cúbica, una curva de bezier cúbica se define por cuatro puntos de control (P0, P1, P2, P3) y un parámetro t que varía entre 0 y 1.
glm::vec3 BezierCubic(
	const glm::vec3& P0, // punto inicial de la curva
	const glm::vec3& P1, // primer punto de control de la curva
	const glm::vec3& P2, // segundo punto de control de la curva
	const glm::vec3& P3, // punto final de la curva
    float t)
{
	float u = 1.0f - t;  //t representa la posicion de la trayectoria t = 0: el modelo está en el punto inicial P0., t = 0.5: el modelo está en una posición intermedia.
    //t = 1: el modelo llega al punto final P3.
    return 
        // B(t) = (1 - t) ^ 3P_0 + 3(1 - t) ^ 2tP_1 + 3(1 - t)t ^ 2P_2 + t ^ 3P_3
        
        u * u * u * P0 + 
        3.0f * u * u * t * P1 +
        3.0f * u * t * t * P2 +
        t * t * t * P3;
}

int main()
{
    // CARGAR EL ARCHIVO glTF 
    tinygltf::Model gltfModel;

    if (!LoadGLTF(SCENE_PATH, gltfModel)) // si no se pudo cargar el archivo, termina el programa
    {
        return -1;
    }

    // CARGAR EL ARCHIVO glTF DEL CASTILLO
    tinygltf::Model castleGLTF;

    if (!LoadGLTF(CASTLE_PATH, castleGLTF))
    {
        std::cout << "No se pudo cargar el castillo."
            << std::endl;

        return -1;
    }

    // CARGAR EL ARCHIVO glTF DE LOS DRAGONES
    tinygltf::Model dragonsGLTF;

    if (!LoadGLTF(DRAGONS_PATH, dragonsGLTF))
    {
        std::cout << "No se pudo cargar la escena de dragones" << std::endl;
        return -1;
    }

    // CARGAR EL ARCHIVO glTF DEL DRAGÓN VOLADOR
    tinygltf::Model flyingDragonGLTF;

    if (!LoadGLTF(FLYINGDRAGON_PATH, flyingDragonGLTF))
    {
        std::cout << "No se pudo cargar el dragón volador" << std::endl;
        return -1;
    }

    // CARGAR EL ARCHIVO glTF DE LOS GUERREROS
    tinygltf::Model warriorsGLTF;

    if (!LoadGLTF(WARRIORS_PATH, warriorsGLTF))
    {
        std::cout << "No se pudo cargar la escena de guerreros" << std::endl;
        return -1;
    }

    std::cout << "Cantidad de meshes: " // coleccion de mallas del modelo 
        << gltfModel.meshes.size() << std::endl;

    std::cout << "Cantidad de materiales: " // cuantos elementos tiene la coleccion de materiales del modelo
        << gltfModel.materials.size() << std::endl;

    std::cout << "Cantidad de nodos: " // muestra la cantidad de nodos del modelo, que representan la jerarquía de objetos en la escena
        << gltfModel.nodes.size() << std::endl;

	std::cout << "Cantidad de texturas: " // muestra la cantidad de texturas cargadas en el modelo, que pueden ser utilizadas para los materiales
        << gltfModel.textures.size() << std::endl;

    std::cout << "Cantidad de imagenes: " // muestra la cantidad de imágenes cargadas en el modelo, que pueden ser utilizadas como texturas para los materiales
        << gltfModel.images.size() << std::endl;

	// CARGAR EL ARCHIVO glTF DE LA LUNA
	tinygltf::Model moonGLTF;
	if (!LoadGLTF(MOON_PATH, moonGLTF))
	{
		std::cout << "No se pudo cargar la escena de la luna" << std::endl;
		return -1;
	}
    std::cout << "\n--- INFORMACION DE LA LUNA ---" << std::endl;

    std::cout << "Meshes: "
        << moonGLTF.meshes.size() << std::endl;

    std::cout << "Materiales: "
        << moonGLTF.materials.size() << std::endl;

    std::cout << "Texturas: "
        << moonGLTF.textures.size() << std::endl;

    std::cout << "Imagenes: "
        << moonGLTF.images.size() << std::endl;

    for (size_t i = 0; i < moonGLTF.images.size(); i++)
    {
        std::cout << "Imagen " << i
            << ": " << moonGLTF.images[i].uri
            << " | Ancho: " << moonGLTF.images[i].width
            << " | Alto: " << moonGLTF.images[i].height
            << std::endl;
    }
    
    // INICIALIZAR GLFW
    //glfwInit() prepara el entorno del sistema operativo (ventanas, monitores,
// teclado/mouse y memoria interna).
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // perfil core (sin funciones obsoletas) 
	//el perfil core de OpenGL es una configuración que excluye funciones obsoletas 
    // y promueve el uso de características modernas y eficientes de OpenGL, 
    // lo que resulta en un código más limpio y optimizado para aplicaciones gráficas actuales.

	// Activar buffer de profundidad, que permite que los objetos más cercanos tapen a los más lejanos.
    glfwWindowHint(GLFW_DEPTH_BITS, 24);


    // CREAR VENTANA

    GLFWwindow* window = glfwCreateWindow(
        800,
        800,
        "Proyecto OpenGL - Escenario glTF",
		NULL, // monitor (NULL = ventana normal)
		NULL // ventana compartida (NULL = no compartir)
    );

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;

        glfwTerminate();

        return -1;
    }

	glfwMakeContextCurrent(window); // hace que el contexto de OpenGL de la ventana sea el actual en el hilo principal

    // Oculta el cursor y lo "atrapa" dentro de la ventana (estilo FPS)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // INICIALIZAR GLAD
    // gladLoad busca y carga las direcciones de memoria de las funciones 
// de OpenGL específicas de tu tarjeta gráfica.
    
    gladLoadGL();
	glViewport(0, 0, 800, 800); // tamaño de la ventana (en píxeles) y del viewport (área de renderizado)


    // DEPTH TEST
    // Habilita el test de profundidad para que los objetos más cercanos tapen a los más lejanos.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    // ---------------------------------------------------------------
    // CONFIGURACIÓN DEL MAPA DE SOMBRAS
    // ---------------------------------------------------------------

	const unsigned int SHADOW_WIDTH = 2048; // Resolución del mapa de sombras (ancho)
	const unsigned int SHADOW_HEIGHT = 2048; // Resolución del mapa de sombras (alto)

    // Framebuffer para almacenar las profundidades
	GLuint depthFBO; // Identificador del framebuffer de profundidad
	glGenFramebuffers(1, &depthFBO); // Genera un framebuffer y almacena su ID en depthFBO

    // Textura de profundidad
	GLuint depthMap; // Identificador de la textura de profundidad
	glGenTextures(1, &depthMap); // Genera una textura y almacena su ID en depthMap

	glBindTexture(GL_TEXTURE_2D, depthMap); // Vincula la textura de profundidad como una textura 2D activa

	//glTexImage2D crea una textura 2D vacía con formato de profundidad (GL_DEPTH_COMPONENT) y 
    // dimensiones SHADOW_WIDTH x SHADOW_HEIGHT.
    glTexImage2D(
		GL_TEXTURE_2D, // Tipo de textura: 2D
		0, // Nivel de detalle (0 = base)
		GL_DEPTH_COMPONENT, // Formato de la textura: componente de profundidad
        SHADOW_WIDTH,
        SHADOW_HEIGHT,
		0, // Borde (debe ser 0)
		GL_DEPTH_COMPONENT, // Formato de los datos de la textura: componente de profundidad
		GL_FLOAT, // Tipo de dato de los datos de la textura: float
        nullptr 
    );

    // Configuración de la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glTexParameterfv(
        GL_TEXTURE_2D,
        GL_TEXTURE_BORDER_COLOR,
        borderColor
    );

    // Asociar la textura al framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);

	glFramebufferTexture2D( // Asocia la textura de profundidad al framebuffer
		GL_FRAMEBUFFER, // Tipo de framebuffer
		GL_DEPTH_ATTACHMENT, // Tipo de attachment: profundidad
        GL_TEXTURE_2D,
		depthMap, // ID de la textura de profundidad
        0
    );

    // No necesitamos almacenar colores
	//por que solo nos interesa la información de profundidad para generar sombras
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // Comprobar que el framebuffer esté completo
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Error: Framebuffer de sombras incompleto." << std::endl;
    }

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Desvincula el framebuffer, volviendo al framebuffer por defecto (ventana)
	glBindTexture(GL_TEXTURE_2D, 0); // Desvincula la textura de profundidad, volviendo a la textura por defecto (0)



    // CREAR EL ESCENARIO
    // unique_ptr para poder destruirlo ANTES de glfwTerminate(): las mallas
    // llaman a glDelete* en su destructor y eso solo es válido mientras el
    // contexto OpenGL siga vivo.

	std::unique_ptr<Model> scene = std::make_unique<Model>(gltfModel); // Crea un objeto Model a partir del modelo glTF cargado previamente

    // CREAR EL CASTILLO
    std::unique_ptr<Model> castle =
        std::make_unique<Model>(castleGLTF);

    // CREAR EL MODELO DE LOS DRAGONES
    std::unique_ptr<Model> dragons =
        std::make_unique<Model>(dragonsGLTF);
    
	// CREAR EL MODELO DE LOS GUERREROS
	std::unique_ptr<Model> warriors =
		std::make_unique<Model>(warriorsGLTF);
    
	// CREAR EL MODELO DE LA LUNA
 // CREAR EL MODELO DE LA LUNA
    std::unique_ptr<Model> moon =
        std::make_unique<Model>(moonGLTF);

    glm::mat4 moonModel = glm::mat4(1.0f);

    float moonRotation = 0.0f;
	
    
    //Crear el modelo del dragon volador

    std::unique_ptr<Model> flyingDragon =
        std::make_unique<Model>(flyingDragonGLTF);
    //------------------------------------------------------
    // DIMENSIONES DEL DRAGÓN VOLADOR
    //------------------------------------------------------

    glm::vec3 flyingDragonSize =
        flyingDragon->boundsMax - flyingDragon->boundsMin;

    glm::vec3 flyingDragonCenter =
        (flyingDragon->boundsMin + flyingDragon->boundsMax) * 0.5f;

    float flyingDragonMaxSide = std::max(
        flyingDragonSize.x,
        std::max(flyingDragonSize.y, flyingDragonSize.z)
    );

// Tamaño deseado del dragón
float flyingDragonTargetSize = 0.5f;

    //------------------------------------------------------
    // PUNTOS DE CONTROL DE LA CURVA DE BEZIER
    //------------------------------------------------------

    glm::vec3 P0(-1.0f, 0.5f, -1.0f); // punto inicial de la curva
	glm::vec3 P1(-0.5f, 2.0f, 0.0f); // punto de control 1
	glm::vec3 P2(1.0f, 2.0f, 1.0f);// punto de control 2
	glm::vec3 P3(2.0f, 0.5f, 0.0f);// punto final de la curva

    //------------------------------------------------------
    // ANIMACIÓN DEL DRAGÓN VOLADOR
    //------------------------------------------------------

	float curveT = 0.0f; //Inicializa el parámetro t de la curva de Bezier en 0.0, que representa el punto inicial de la curva.
	float curveSpeed = 0.15f; //Velocidad a la que el dragón se mueve a lo largo de la curva de Bezier.
	float curveDirection = 1.0f; //Dirección del movimiento a lo largo de la curva de Bezier. Un valor positivo (1.0) indica que el dragón se mueve hacia adelante a lo largo de la curva, 
    //mientras que un valor negativo (-1.0) indica que se mueve hacia atrás.

	Shader modelShader("model.vert", "model.frag"); // Crea un objeto Shader para el shader de modelos, utilizando los archivos "model.vert" y "model.frag" como shaders de vértices y fragmentos, respectivamente.
	Shader shadowShader("shadow.vert", "shadow.frag"); // Crea un objeto Shader para el shader de sombras, utilizando los archivos "shadow.vert" y "shadow.frag" como shaders de vértices y fragmentos, respectivamente.


    // VARIABLES DE UNIFORMES
	GLint uMVP = glGetUniformLocation(modelShader.ID, "uMVP"); // Obtiene la ubicación del uniforme "uMVP" en el shader de modelos, que se utilizará para enviar la matriz de transformación completa (Modelo-Vista-Proyección) al shader.
	GLint uModel = glGetUniformLocation(modelShader.ID, "uModel");// Obtiene la ubicación del uniforme "uModel" en el shader de modelos, que se utilizará para enviar la matriz de transformación del modelo al shader.

    // Uniformes para iluminación Phong
	GLint uViewPos = glGetUniformLocation(modelShader.ID, "uViewPos"); // Obtiene la ubicación del uniforme "uViewPos" en el shader de modelos, que se utilizará para enviar la posición de la cámara al shader para cálculos de iluminación.
	GLint uShininess = glGetUniformLocation(modelShader.ID, "uShininess"); // Obtiene la ubicación del uniforme "uShininess" en el shader de modelos, que se utilizará para enviar el valor de brillo especular al shader para cálculos de iluminación.

    // CÁMARA
    // Empieza dentro del escenario, a la altura de los ojos. Ajusta la posición
    // si quedas dentro de una pared o muy lejos.

	Camera camera(800, 800, glm::vec3(0.0f, 1.5f, 3.0f)); // Crea un objeto Camera con una resolución de 800x800 píxeles y una posición inicial en el espacio 3D en (0.0, 1.5, 3.0), que representa la posición de la cámara en el mundo.

	float lastFrame = 0.0f; // Variable para almacenar el tiempo del último frame renderizado, utilizada para calcular el tiempo transcurrido entre frames y ajustar la velocidad de animación de manera independiente a la tasa de frames por segundo (FPS).


    // MATRIZ DEL ESCENARIO (se calcula una sola vez)
    // 1. Se centra en X y Z con el piso (el punto más bajo) en y = 0.
    // 2. Se escala para que su lado más largo mida SCENE_TARGET_SIZE.

	glm::vec3 size = scene->boundsMax - scene->boundsMin; // Dimensiones del escenario
	glm::vec3 center = (scene->boundsMin + scene->boundsMax) * 0.5f; // Centro geométrico del escenario
	float maxSide = std::max(size.x, std::max(size.y, size.z)); // Lado más largo del escenario

	glm::mat4 sceneModel = glm::mat4(1.0f); // Matriz de transformación del escenario, inicializada como la matriz identidad
	sceneModel = glm::scale(sceneModel, glm::vec3(SCENE_TARGET_SIZE / maxSide)); // Escala el escenario para que su lado más largo mida SCENE_TARGET_SIZE, manteniendo la proporción de las dimensiones originales.
	sceneModel = glm::translate(sceneModel, glm::vec3(-center.x, -scene->boundsMin.y, -center.z)); // Centra el escenario en X y Z, y coloca su punto más bajo en y = 0, ajustando la posición del escenario en el espacio 3D.



    // ---------------------------------------------------------------
    // TRANSFORMACIÓN DEL CASTILLO
    // ---------------------------------------------------------------

    // Obtener dimensiones del castillo
    glm::vec3 castleSize =
		castle->boundsMax - castle->boundsMin; // Dimensiones del castillo

    // Centro del castillo
    glm::vec3 castleCenter =
		(castle->boundsMin + castle->boundsMax) * 0.5f; // Centro geométrico del castillo

    // Lado más largo
    float castleMaxSide =
        std::max(
            castleSize.x,
			std::max(castleSize.y, castleSize.z) // Calcula el lado más largo del castillo comparando sus dimensiones en X, Y y Z
        );

    // Crear matriz de transformación
	glm::mat4 castleModel(1.0f); // Inicializa la matriz de transformación del castillo como la matriz identidad

    castleModel = glm::translate(
        castleModel,
        //Las coordenadas del castillo se ajustan para que esté centrado en el eje X y Z, y apoyado en el piso (y=0)
         glm::vec3(0.25f, 0.001f, 0.75f)
    );

    // Rotación del castillo
    castleModel = glm::rotate(
        castleModel,
		glm::radians(195.0f), // Convierte 195 grados a radianes para la rotación
        glm::vec3(0.0f, 1.0f, 0.0f)
    );


    // Escala del castillo
    castleModel = glm::scale(
        castleModel,
        glm::vec3(CASTLE_TARGET_SIZE / castleMaxSide)
    );


    // Centrar el modelo y apoyar su base
    castleModel = glm::translate(
        castleModel,
        glm::vec3(
            -castleCenter.x,
            -castle->boundsMin.y,
            -castleCenter.z
        )
    );

    // ---------------------------------------------------------------
    // TRANSFORMACIÓN DE LOS DRAGONES
    //----------------------------------------------------------------

    // Transformación de los dragones
    glm::mat4 dragonsModel = glm::mat4(1.0f);

    // Cambiar posición
    dragonsModel = glm::translate(
        dragonsModel,
        glm::vec3(-0.5f, 0.15f, 0.0f)
    );

    // Cambiar rotación
    dragonsModel = glm::rotate(
        dragonsModel,
        glm::radians(15.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // Cambiar escala
    dragonsModel = glm::scale(
        dragonsModel,
		glm::vec3(0.01f, 0.01f, 0.01f)
    );
	//---------------------------------------------------------------
	//TRANSFORMACIÓN DE LOS GUERREROS
	//--------------------------------------------------------------- 
    // Transformación de los guerreros
    glm::mat4 warriorsModel = glm::mat4(1.0f);

  

    // Cambiar posición
    warriorsModel = glm::translate(
        warriorsModel,
        glm::vec3(0.1f, 0.05f, 0.65f)
    );
    
    // Cambiar rotación
    warriorsModel = glm::rotate(
        warriorsModel,
        glm::radians(220.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
  

    // Cambiar escala
    warriorsModel = glm::scale(
        warriorsModel,
        glm::vec3(0.05f, 0.05f, 0.05f)
    );


    // ---------------------------------------------------------------
    // TRANSFORMACIÓN DE LA LUNA
    // ---------------------------------------------------------------

    // Obtener dimensiones de la luna
    glm::vec3 moonSize =
        moon->boundsMax - moon->boundsMin;

    // Centro geométrico de la luna
    glm::vec3 moonCenter =
        (moon->boundsMin + moon->boundsMax) * 0.5f;

    // Lado más largo
    float moonMaxSide =
        std::max(
            moonSize.x,
            std::max(moonSize.y, moonSize.z)
        );

    // Posición de la luna en el escenario
    glm::vec3 moonPosition(0.0f, 0.5f, -0.5f);

    // Tamaño deseado
    float moonTargetSize = 0.15f;

    // Matriz inicial
    moonModel = glm::translate(
        glm::mat4(1.0f),
        moonPosition
    );

    moonModel = glm::scale(
        moonModel,
        glm::vec3(moonTargetSize / moonMaxSide)
    );

    moonModel = glm::translate(
        moonModel, 
        -moonCenter
    );
   
   
	// ---------------------------------------------------------------
    // CREAR CUBO DE PRUEBA
    std::unique_ptr<Cube> cube = std::make_unique<Cube>();

    glm::mat4 cubeModel(1.0f);

    // Posición que anteriormente ocupaba el castillo
    cubeModel = glm::translate(
        cubeModel,
        glm::vec3(2.0f, 0.0f, 0.0f)
    );

    cubeModel = glm::scale(
        cubeModel,
        glm::vec3(0.7f)
    );

    // ---------------------------------------------------------------
    // CREAR ESFERA DE PRUEBA PARA ILUMINACION PHONG
    // ---------------------------------------------------------------

    std::unique_ptr<Sphere> sphere =
        std::make_unique<Sphere>();

    glm::mat4 sphereModel(1.0f);

    // Posicion de la esfera
    sphereModel = glm::translate(
        sphereModel,
        glm::vec3(-3.0f, 0.8f, 0.0f)
    );

    // Tamaño de la esfera
    sphereModel = glm::scale(
        sphereModel,
        glm::vec3(0.8f)
    );



    // ---------------------------------------------------------------
    // CONFIGURACIÓN DE LA LUZ PARA LAS SOMBRAS
    // ---------------------------------------------------------------

    // Centro del escenario en coordenadas locales
    glm::vec3 worldCenter =
        glm::vec3(sceneModel * glm::vec4(center, 1.0f));

    // Dirección de la luz
    glm::vec3 lightDirection =
        glm::normalize(glm::vec3(1.4f, 1.0f, 0.3f));

    // Posición de la luz
    glm::vec3 lightPosition =
        worldCenter + lightDirection * 15.0f;

    // Vista desde la luz
    glm::mat4 lightView =
        glm::lookAt(
            lightPosition,
            worldCenter,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

    // Proyección ortográfica de la luz
	// Se ajusta para que abarque todo el escenario
	//ortografica significa que los rayos de luz son paralelos, 
    // lo que es útil para simular la luz del sol y generar sombras consistentes en toda la escena.
    glm::mat4 lightProjection =
        glm::ortho(
			-8.0f, 8.0f,// izquierda, derecha
			-8.0f, 8.0f, // abajo, arriba
			0.1f, 40.0f //Cerca, lejos
        );

    // Matriz final de iluminación
    glm::mat4 lightSpaceMatrix =
        lightProjection * lightView;


    //-------------// RENDER LOOP //----------------------------------------

    while (!glfwWindowShouldClose(window))
    {
        // DELTA TIME (para que la velocidad no dependa del FPS)
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        //------------------------------------------------------
        // CURVA DE BEZIER PARA EL MOVIMIENTO DEL DRAGÓN
        //------------------------------------------------------

        curveT += curveSpeed * deltaTime * curveDirection;

		if (curveT >= 1.0f) //curveT representa la posición del dragón a lo largo de la curva de Bezier,
        {
            curveT = 1.0f;
            curveDirection = -1.0f;
        }

        if (curveT <= 0.0f)
        {
            curveT = 0.0f;
            curveDirection = 1.0f;
        }

        // Calcular posición del dragón volador
        glm::vec3 flyingDragonPosition = BezierCubic(
            P0,
            P1,
            P2,
            P3,
            curveT
        );

        //------------------------------------------------------
        // TRANSFORMACIÓN DEL DRAGÓN VOLADOR
        //------------------------------------------------------

        glm::mat4 flyingDragonModel = glm::mat4(1.0f);

        // Cambiar posición
        flyingDragonModel = glm::translate(
            flyingDragonModel,
            flyingDragonPosition
        );

        // Cambiar escala
        flyingDragonModel = glm::scale(
            flyingDragonModel,
            glm::vec3(
                flyingDragonTargetSize / flyingDragonMaxSide
            )
        );

        // Centrar modelo
        flyingDragonModel = glm::translate(
            flyingDragonModel,
            -flyingDragonCenter
        );
		//_--------------------------------------------------------------
 

        // ---------------------------------------------------------------
        // ROTACIÓN DE LA LUNA
        // ---------------------------------------------------------------

        // Velocidad de rotación: 20 grados por segundo
        moonRotation += glm::radians(20.0f) * deltaTime;

        // Evitar que el ángulo crezca indefinidamente
        if (moonRotation >= glm::radians(360.0f))
        {
            moonRotation -= glm::radians(360.0f);
        }

        // Construir nuevamente la matriz de transformación
        moonModel = glm::translate(
            glm::mat4(1.0f),
            moonPosition
        );

        moonModel = glm::rotate(
            moonModel,
            moonRotation,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        moonModel = glm::scale(
            moonModel,
            glm::vec3(moonTargetSize / moonMaxSide)
        );

        moonModel = glm::translate(
            moonModel,
            -moonCenter
        );


        // INPUT DE CÁMARA

        camera.Inputs(window, deltaTime);   // teclado
        camera.MouseInputs(window);         // mouse (yaw/pitch)

        // ESC cierra el programa
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }


        // ---------------------------------------------------------------
        // PRIMERA PASADA: GENERAR EL MAPA DE PROFUNDIDA
        // // ---------------------------------------------------------------
         
        
        // Utilizar la resolución del mapa de sombras
         glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

        // Activar el framebuffer de profundidad
        glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);

        // Limpiar el buffer de profundidad
        glClear(GL_DEPTH_BUFFER_BIT);

        // Activar el shader de sombras
        shadowShader.Activate();

        // Dibujar el escenario desde la perspectiva de la luz
        scene->DrawDepth(
            shadowShader.ID,
            gltfModel,
            lightSpaceMatrix,
            sceneModel
        );

        // Generar sombras del castillo
        castle->DrawDepth(
			shadowShader.ID,// ID del shader de sombras
			castleGLTF,// Modelo glTF del castillo
			lightSpaceMatrix,// Matriz de transformación de la luz
			castleModel// Matriz de transformación del castillo
        );

        // Generar sombras de los dragones
        dragons->DrawDepth(
            shadowShader.ID,
            dragonsGLTF,
            lightSpaceMatrix,
            dragonsModel
        );

        // Generar sombras de los guerreros
        warriors->DrawDepth(
            shadowShader.ID,
            warriorsGLTF,
            lightSpaceMatrix,
            warriorsModel
        );
        
      // Generar sombras de la luna
        moon->DrawDepth(
            shadowShader.ID,
            moonGLTF,
            lightSpaceMatrix,
            moonModel
        );

        // Generar sombras del dragón volador
        flyingDragon->DrawDepth(
            shadowShader.ID,
            flyingDragonGLTF,
            lightSpaceMatrix,
            flyingDragonModel
        );

        // Dibujar el cubo en el mapa de profundidad
        glm::mat4 cubeLightMVP = lightSpaceMatrix * cubeModel;

        glUniformMatrix4fv(
            glGetUniformLocation(shadowShader.ID, "uLightMVP"),
            1,
            GL_FALSE,
            glm::value_ptr(cubeLightMVP)
        );

		// cube->Draw(); // 

        // Dibujar esfera en el mapa de profundidad
        glm::mat4 sphereLightMVP =
            lightSpaceMatrix * sphereModel;

        glUniformMatrix4fv(
            glGetUniformLocation(shadowShader.ID, "uLightMVP"),
            1,
            GL_FALSE,
            glm::value_ptr(sphereLightMVP)
        );

		// sphere->Draw(); // Descomenta esta línea si quieres que la esfera genere sombras

		
        //SEGUNDA PASADA: DIBUJAR ESCENA CON SOMBRAS

        // Volver al framebuffer principal
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Restaurar la resolución de la ventana
        glViewport(0, 0, 800, 800);


        // LIMPIAR PANTALLA

        glClearColor(0.015f, 0.025f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // MATRICES DE VISTA Y PROYECCIÓN

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix(45.0f, 0.1f, 100.0f);


        // DIBUJAR EL ESCENARIO

		modelShader.Activate(); // activar el shader del modelo

        // Posición actual de la cámara
        glUniform3fv(
            uViewPos,
            1,
            glm::value_ptr(camera.Position)
        );

        // Concentración del reflejo especular
        glUniform1f(uShininess, 32.0f);


        // Enviar matriz de iluminación
        glUniformMatrix4fv(
            glGetUniformLocation(modelShader.ID, "uLightSpaceMatrix"),
            1,
            GL_FALSE,
            glm::value_ptr(lightSpaceMatrix)
        );

        // Enviar mapa de sombras a la unidad de textura 1
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        glUniform1i(
            glGetUniformLocation(modelShader.ID, "uShadowMap"),
            1
        );

		glm::mat4 mvp = projection * view * sceneModel; // Matriz final de transformación (MVP)

		glUniformMatrix4fv(uMVP, 1, GL_FALSE, glm::value_ptr(mvp)); // Enviar la matriz MVP al shader
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(sceneModel)); // Enviar la matriz de modelo al shader

        glm::mat4 viewProjection = projection * view;
		
        // Dibujar el escenario
        scene->Draw(
            modelShader.ID,
            gltfModel,
            viewProjection,
            sceneModel
        );

		// Dibujar el castillo
    
        castle->Draw(
            modelShader.ID,
            castleGLTF,
            viewProjection,
            castleModel
        );
		// Dibujar los dragones
        dragons->Draw(
            modelShader.ID,
            dragonsGLTF,
            viewProjection,
            dragonsModel

        );

        // Dibujar los guerreros
        warriors->Draw(
            modelShader.ID,
            warriorsGLTF,
            viewProjection,
            warriorsModel
        );

        // Dibujar la luna
        moon->Draw(
            modelShader.ID,
            moonGLTF,
            viewProjection,
            moonModel
        );

        // Dibujar el dragón volador
        flyingDragon->Draw(
            modelShader.ID,
            flyingDragonGLTF,
            viewProjection,
            flyingDragonModel
        );

        // ---------------------------------------------------------------
        // DIBUJAR CUBO DE PRUEBA
        // ---------------------------------------------------------------

        glm::mat4 cubeMVP = viewProjection * cubeModel;

        // Enviar matrices al shader
        glUniformMatrix4fv(
            uMVP,
            1,
            GL_FALSE,
            glm::value_ptr(cubeMVP)
        );

        glUniformMatrix4fv(
            uModel,
            1,
            GL_FALSE,
            glm::value_ptr(cubeModel)
        );

        // Color azul para distinguir el cubo
        glUniform4f(
            glGetUniformLocation(modelShader.ID, "uBaseColor"),
            0.15f, 0.35f, 0.85f, 1.0f
        );

        // Indicar que el cubo no utiliza textura
        glUniform1i(
            glGetUniformLocation(modelShader.ID, "uHasTexture"),
            0
        );

        // Desactivar recorte por transparencia
        glUniform1f(
            glGetUniformLocation(modelShader.ID, "uAlphaCutoff"),
            0.0f
        );

        // Dibujar cubo
         //cube->Draw();

        // ---------------------------------------------------------------
        // DIBUJAR ESFERA DE PRUEBA PHONG
        // ---------------------------------------------------------------

        glm::mat4 sphereMVP =
            viewProjection * sphereModel;

        // Enviar matrices al shader
        glUniformMatrix4fv(
            uMVP,
            1,
            GL_FALSE,
            glm::value_ptr(sphereMVP)
        );

        glUniformMatrix4fv(
            uModel,
            1,
            GL_FALSE,
            glm::value_ptr(sphereModel)
        );

        // Color gris claro para observar los reflejos
        glUniform4f(
            glGetUniformLocation(modelShader.ID, "uBaseColor"),
            0.75f, 0.78f, 0.82f, 1.0f
        );

        // La esfera no utiliza textura
        glUniform1i(
            glGetUniformLocation(modelShader.ID, "uHasTexture"),
            0
        );

        // Desactivar recorte por transparencia
        glUniform1f(
            glGetUniformLocation(modelShader.ID, "uAlphaCutoff"),
            0.0f
        );

        // Dibujar esfera
         //sphere->Draw();


        // TERMINAR FRAME

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // LIBERAR RECURSOS (siempre ANTES de destruir la ventana y terminar GLFW)
    
    
    // LIBERAR RECURSOS 
	sphere.reset(); // destruye la esfera con el contexto aún vivo

	castle.reset(); // destruye el castillo con el contexto aún vivo

	dragons.reset(); // destruye los dragones con el contexto aún vivo

	warriors.reset(); // destruye los guerreros con el contexto aún vivo

	moon.reset();// destruye la luna con el contexto aún vivo

    flyingDragon.reset();

	cube.reset(); // destruye el cubo con el contexto aún vivo
 
    scene.reset(); // destruye las mallas con el contexto aún vivo

    modelShader.Delete();
    shadowShader.Delete();// destruir shaders


	glDeleteFramebuffers(1, &depthFBO); // destruir el framebuffer de sombras
	glDeleteTextures(1, &depthMap); // destruir la textura de profundidad

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}