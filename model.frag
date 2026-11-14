
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

    // Iluminación ambiental fría
    vec3 ambientColor = vec3(0.43, 0.49, 0.60);
    vec3 ambient = base.rgb * ambientColor;

    // Iluminación difusa
    float diffuseFactor = max(dot(normal, lightDir), 0.0);

    vec3 lightColor = vec3(0.94, 0.97, 1.0);
    vec3 diffuse = base.rgb * lightColor * diffuseFactor * 0.65;

    // Resultado final
    vec3 finalColor = ambient + diffuse;

    FragColor = vec4(finalColor, base.a);
}
