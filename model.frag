
#version 330 core

out vec4 FragColor;

in vec3 vNormal;
in vec2 vTex;
in vec4 vPosLightSpace;

uniform sampler2D uTex;
uniform sampler2D uShadowMap;

uniform vec4 uBaseColor;
uniform int uHasTexture;
uniform float uAlphaCutoff;

// ---------------------------------------------------------------
// CALCULAR SOMBRAS
// ---------------------------------------------------------------

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // Convertir coordenadas de luz a coordenadas de textura
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;

    // Fuera del mapa de sombras
    if (projCoords.z > 1.0 ||
        projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0)
    {
        return 0.0;
    }

    // Bias para evitar sombras incorrectas sobre la propia superficie
    float bias = max(
        0.003 * (1.0 - dot(normal, lightDir)),
        0.0005
    );

    // Tamaño de un texel del mapa de sombras
    vec2 texelSize = 1.0 / textureSize(uShadowMap, 0);

    // PCF: suavizar los bordes de las sombras
    float shadow = 0.0;

    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            float closestDepth = texture(
                uShadowMap,
                projCoords.xy + vec2(x, y) * texelSize
            ).r;

            float currentDepth = projCoords.z;

            if (currentDepth - bias > closestDepth)
                shadow += 1.0;
        }
    }

    shadow /= 9.0;

    return shadow;
}

// ---------------------------------------------------------------
// ILUMINACIÓN PRINCIPAL
// ---------------------------------------------------------------

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

    vec3 lightColor = vec3(1.0, 0.98, 0.95);

    vec3 diffuse =
        base.rgb * lightColor * diffuseFactor * 0.78;

    // Calcular sombras
    float shadow = ShadowCalculation(
        vPosLightSpace,
        normal,
        lightDir
    );

    // Aplicar sombras principalmente a la luz directa
    vec3 finalColor =
        ambient + diffuse * (1.0 - shadow * 0.75);

    FragColor = vec4(finalColor, base.a);
}
