#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

uniform mat4 uMVP; // Matriz de transformación completa (Model-View-Projection)
uniform mat4 uModel; // Matriz de transformación del modelo
uniform mat4 uLightSpaceMatrix; // Matriz de transformación de la luz

out vec3 vNormal; // Nueva variable: normal transformada
out vec2 vTex; // Nueva variable: coordenadas de textura, utilizadas para el mapeo de sombras
out vec4 vPosLightSpace; // Nueva variable: posición desde la perspectiva de la luz

// Nueva variable: posición del fragmento en el mundo
out vec3 vFragPos;

void main()
{
    gl_Position = uMVP * vec4(aPos, 1.0); //

    // Posición del vértice en coordenadas del mundo
    vec4 worldPosition = uModel * vec4(aPos, 1.0);
    vFragPos = worldPosition.xyz;

    // Transformación de la normal
    vNormal = mat3(transpose(inverse(uModel))) * aNormal;

    vTex = aTex;

    // Posición desde la perspectiva de la luz
    vPosLightSpace = uLightSpaceMatrix * worldPosition;
}