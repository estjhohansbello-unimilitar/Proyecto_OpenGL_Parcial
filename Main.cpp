#include <iostream>
#include <algorithm> // std::max
#include <memory>    // std::unique_ptr

#include <glad/glad.h> // para cargar las funciones de OpenGL
#include <GLFW/glfw3.h> // para crear la ventana y el contexto de OpenGL

#include <glm/glm.hpp> // permite trabajar con vectores y matrices
#include <glm/gtc/matrix_transform.hpp> // matrices de transformación (traslación, rotación, escalado, proyección)
#include <glm/gtc/type_ptr.hpp> // permite convertir matrices y vectores a punteros de tipo float

#include "shaderClass.h"
#include "Camera.h"

#include "Model.h"
#include "ModelLoader.h"


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
const float CASTLE_TARGET_SIZE = 0.05f;

// RUTA DE LA ESCENA DE DRAGONES
const char* DRAGONS_PATH = "Models/Dragons/scene.gltf";

// RUTA DE LA ESCENA DE GUERREROS
const char* WARRIORS_PATH = "Models/Warriors/scene.gltf";


// Tamaño al que se escala el lado más largo del escenario, en unidades del mundo.
const float SCENE_TARGET_SIZE = 2.5f;


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

    std::cout << "Cantidad de texturas: "
        << gltfModel.textures.size() << std::endl;

    std::cout << "Cantidad de imagenes: " // muestra la cantidad de imágenes cargadas en el modelo, que pueden ser utilizadas como texturas para los materiales
        << gltfModel.images.size() << std::endl;


    // INICIALIZAR GLFW
    //glfwInit() prepara el entorno del sistema operativo (ventanas, monitores,
// teclado/mouse y memoria interna).
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // perfil core (sin funciones obsoletas)

    // Activar buffer de profundidad
    glfwWindowHint(GLFW_DEPTH_BITS, 24);


    // CREAR VENTANA

    GLFWwindow* window = glfwCreateWindow(
        800,
        800,
        "Proyecto OpenGL - Escenario glTF",
        NULL,
        NULL
    );

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;

        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window);

    // Oculta el cursor y lo "atrapa" dentro de la ventana (estilo FPS)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // INICIALIZAR GLAD
    // gladLoad busca y carga las direcciones de memoria de las funciones 
// de OpenGL específicas de tu tarjeta gráfica.
    gladLoadGL();
    glViewport(0, 0, 800, 800);


    // DEPTH TEST
    // Habilita el test de profundidad para que los objetos más cercanos tapen a los más lejanos.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    // ---------------------------------------------------------------
    // CONFIGURACIÓN DEL MAPA DE SOMBRAS
    // ---------------------------------------------------------------

    const unsigned int SHADOW_WIDTH = 2048;
    const unsigned int SHADOW_HEIGHT = 2048;

    // Framebuffer para almacenar las profundidades
    GLuint depthFBO;
    glGenFramebuffers(1, &depthFBO);

    // Textura de profundidad
    GLuint depthMap;
    glGenTextures(1, &depthMap);

    glBindTexture(GL_TEXTURE_2D, depthMap);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_DEPTH_COMPONENT,
        SHADOW_WIDTH,
        SHADOW_HEIGHT,
        0,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
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

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        depthMap,
        0
    );

    // No necesitamos almacenar colores
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // Comprobar que el framebuffer esté completo
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Error: Framebuffer de sombras incompleto." << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);



    // CREAR EL ESCENARIO
    // unique_ptr para poder destruirlo ANTES de glfwTerminate(): las mallas
    // llaman a glDelete* en su destructor y eso solo es válido mientras el
    // contexto OpenGL siga vivo.

    std::unique_ptr<Model> scene = std::make_unique<Model>(gltfModel);

    // CREAR EL CASTILLO
    std::unique_ptr<Model> castle =
        std::make_unique<Model>(castleGLTF);

    // CREAR EL MODELO DE LOS DRAGONES
    std::unique_ptr<Model> dragons =
        std::make_unique<Model>(dragonsGLTF);
    
	// CREAR EL MODELO DE LOS GUERREROS
	std::unique_ptr<Model> warriors =
		std::make_unique<Model>(warriorsGLTF);
    
    // SHADER



    Shader modelShader("model.vert", "model.frag");
    Shader shadowShader("shadow.vert", "shadow.frag");

    // VARIABLES DE UNIFORMES
    GLint uMVP = glGetUniformLocation(modelShader.ID, "uMVP");
    GLint uModel = glGetUniformLocation(modelShader.ID, "uModel");

    // Uniformes para iluminación Phong
    GLint uViewPos = glGetUniformLocation(modelShader.ID, "uViewPos");
    GLint uShininess = glGetUniformLocation(modelShader.ID, "uShininess");

    // CÁMARA
    // Empieza dentro del escenario, a la altura de los ojos. Ajusta la posición
    // si quedas dentro de una pared o muy lejos.

    Camera camera(800, 800, glm::vec3(0.0f, 1.5f, 3.0f));

    float lastFrame = 0.0f;


    // MATRIZ DEL ESCENARIO (se calcula una sola vez)
    // 1. Se centra en X y Z con el piso (el punto más bajo) en y = 0.
    // 2. Se escala para que su lado más largo mida SCENE_TARGET_SIZE.

    glm::vec3 size = scene->boundsMax - scene->boundsMin;
    glm::vec3 center = (scene->boundsMin + scene->boundsMax) * 0.5f;
    float maxSide = std::max(size.x, std::max(size.y, size.z));

    glm::mat4 sceneModel = glm::mat4(1.0f);
    sceneModel = glm::scale(sceneModel, glm::vec3(SCENE_TARGET_SIZE / maxSide));
    sceneModel = glm::translate(sceneModel, glm::vec3(-center.x, -scene->boundsMin.y, -center.z));



    // ---------------------------------------------------------------
    // TRANSFORMACIÓN DEL CASTILLO
    // ---------------------------------------------------------------

    // Obtener dimensiones del castillo
    glm::vec3 castleSize =
        castle->boundsMax - castle->boundsMin;

    // Centro del castillo
    glm::vec3 castleCenter =
        (castle->boundsMin + castle->boundsMax) * 0.5f;

    // Lado más largo
    float castleMaxSide =
        std::max(
            castleSize.x,
            std::max(castleSize.y, castleSize.z)
        );

    // Crear matriz de transformación
    glm::mat4 castleModel(1.0f);

    // Posición que anteriormente ocupaba el cubo
    castleModel = glm::translate(
        castleModel,
        //Las coordenadas del castillo se ajustan para que esté centrado en el eje X y Z, y apoyado en el piso (y=0)
        glm::vec3(0.0f, 0.1f, -0.01f)
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
        glm::vec3(0.0f, 0.0f, 0.0f)
    );

    // Cambiar rotación
    dragonsModel = glm::rotate(
        dragonsModel,
        glm::radians(150.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // Cambiar escala
    dragonsModel = glm::scale(
        dragonsModel,
		glm::vec3(0.005f, 0.005f, 0.005f)
    );
	//---------------------------------------------------------------
	//TRANSFORMACIÓN DE LOS GUERREROS
	//--------------------------------------------------------------- 
    // Transformación de los guerreros
    glm::mat4 warriorsModel = glm::mat4(1.0f);

    // Cambiar posición
    warriorsModel = glm::translate(
        warriorsModel,
        glm::vec3(0.0f, 0.0f, 0.0f)
    );

    // Cambiar rotación
    warriorsModel = glm::rotate(
        warriorsModel,
        glm::radians(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    // Cambiar escala
    warriorsModel = glm::scale(
        warriorsModel,
        glm::vec3(0.05f, 0.05f, 0.05f)
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
    glm::mat4 lightProjection =
        glm::ortho(
            -8.0f, 8.0f,
            -8.0f, 8.0f,
            0.1f, 40.0f
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
            shadowShader.ID,
            castleGLTF,
            lightSpaceMatrix,
            castleModel
        );

        // Dibujar el cubo en el mapa de profundidad
        glm::mat4 cubeLightMVP = lightSpaceMatrix * cubeModel;

        glUniformMatrix4fv(
            glGetUniformLocation(shadowShader.ID, "uLightMVP"),
            1,
            GL_FALSE,
            glm::value_ptr(cubeLightMVP)
        );

		// cube->Draw(); // Descomenta esta línea si quieres que el cubo genere sombras

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