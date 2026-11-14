#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

uniform mat4 uMVP;
uniform mat4 uModel;
uniform mat4 uLightSpaceMatrix;

out vec3 vNormal;
out vec2 vTex;
out vec4 vPosLightSpace;

// Nueva variable: posición del fragmento en el mundo
out vec3 vFragPos;

void main()
{
    gl_Position = uMVP * vec4(aPos, 1.0);

    // Posición del vértice en coordenadas del mundo
    vec4 worldPosition = uModel * vec4(aPos, 1.0);
    vFragPos = worldPosition.xyz;

    // Transformación de la normal
    vNormal = mat3(transpose(inverse(uModel))) * aNormal;

    vTex = aTex;

    // Posición desde la perspectiva de la luz
    vPosLightSpace = uLightSpaceMatrix * worldPosition;
}