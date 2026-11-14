
#version 330 core

out vec4 FragColor;

in vec3 vNormal;
in vec2 vTex;

uniform sampler2D uTex;
uniform vec4 uBaseColor;
uniform int uHasTexture;
uniform float uAlphaCutoff;

void main()
{
    // Color original del material
    vec4 base = uBaseColor;

    if (uHasTexture != 0)
        base *= texture(uTex, vTex);

    if (base.a < uAlphaCutoff)
        discard;

    // Normal de la superficie
    vec3 normal = normalize(vNormal);

    if (!gl_FrontFacing)
        normal = -normal;

    // Dirección de la luz
    vec3 lightDir = normalize(vec3(0.4, 1.0, 0.3));

    // Iluminación ambiental moderada
    vec3 ambientColor = vec3(0.34, 0.36, 0.40);
    vec3 ambient = base.rgb * ambientColor;

    // Iluminación direccional
    float diffuseFactor = max(dot(normal, lightDir), 0.0);

    // Diferenciar superficies iluminadas y sombreadas
    diffuseFactor = pow(diffuseFactor, 1.25);

    vec3 lightColor = vec3(1.0, 0.98, 0.95);
    vec3 diffuse = base.rgb * lightColor * diffuseFactor * 0.78;

    // Resultado final
    vec3 finalColor = ambient + diffuse;

    FragColor = vec4(finalColor, base.a);
}
