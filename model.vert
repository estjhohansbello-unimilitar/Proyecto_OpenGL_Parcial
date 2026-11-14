
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

uniform mat4 uMVP;
uniform mat4 uModel;

// Nueva matriz de iluminación
uniform mat4 uLightSpaceMatrix;

out vec3 vNormal;
out vec2 vTex;

// Nueva variable: posición desde la perspectiva de la luz
out vec4 vPosLightSpace;

void main()
{
    gl_Position = uMVP * vec4(aPos, 1.0);

    vNormal = mat3(transpose(inverse(uModel))) * aNormal;
    vTex = aTex;

    // Transformar la posición al espacio de la luz
    vPosLightSpace =
        uLightSpaceMatrix * uModel * vec4(aPos, 1.0);
}
