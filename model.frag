#version 330 core

out vec4 FragColor;

in vec3 vNormal;
in vec2 vTex;
in vec4 vPosLightSpace;
in vec3 vFragPos;

uniform sampler2D uTex;
uniform sampler2D uShadowMap;

uniform vec4 uBaseColor;
uniform int uHasTexture;
uniform float uAlphaCutoff;
uniform int uUnlit;

uniform vec3 uViewPos;
uniform float uShininess;


// ---------------------------------------------------------------
// CALCULAR SOMBRAS
// ---------------------------------------------------------------

float ShadowCalculation(
    vec4 fragPosLightSpace,
    vec3 normal,
    vec3 lightDir
)
{
    vec3 projCoords =
        fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0 ||
        projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0)
    {
        return 0.0;
    }

    float bias = max(
        0.003 * (1.0 - dot(normal, lightDir)),
        0.0005
    );

    vec2 texelSize =
        1.0 / textureSize(uShadowMap, 0);

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
            {
                shadow += 1.0;
            }
        }
    }

    shadow /= 9.0;

    return shadow;
}


// ---------------------------------------------------------------
// ILUMINACIÓN PHONG
// ---------------------------------------------------------------

void main()
{
    // Color base del material
    vec4 base = uBaseColor;

    // Aplicar textura
    if (uHasTexture != 0)
    {
        base *= texture(uTex, vTex);
    }

    // Recorte por transparencia
    if (base.a < uAlphaCutoff)
    {
        discard;
    }

    // -----------------------------------------------------------
    // MATERIAL SIN ILUMINACIÓN
    // -----------------------------------------------------------

    if (uUnlit != 0)
    {
        FragColor = base;
        return;
    }

    // Normal de la superficie
    vec3 normal = normalize(vNormal);

    if (!gl_FrontFacing)
    {
        normal = -normal;
    }

    // Dirección de la luz
    vec3 lightDir =
        normalize(vec3(1.4, 1.0, 0.3));

    // Dirección hacia la cámara
    vec3 viewDir =
        normalize(uViewPos - vFragPos);

    // -----------------------------------------------------------
    // COMPONENTE AMBIENTAL
    // -----------------------------------------------------------

    vec3 ambientColor =
        vec3(0.34, 0.36, 0.40);

    vec3 ambient =
        base.rgb * ambientColor;

    // -----------------------------------------------------------
    // COMPONENTE DIFUSA
    // -----------------------------------------------------------

    float diffuseFactor =
        max(dot(normal, lightDir), 0.0);

    vec3 lightColor =
        vec3(1.0, 0.98, 0.95);

    vec3 diffuse =
        base.rgb * lightColor *
        diffuseFactor * 0.78;

    // -----------------------------------------------------------
    // COMPONENTE ESPECULAR
    // -----------------------------------------------------------

    vec3 reflectDir =
        reflect(-lightDir, normal);

    float specularFactor =
        pow(
            max(dot(viewDir, reflectDir), 0.0),
            uShininess
        );

    vec3 specular =
        lightColor * specularFactor * 0.35;

    // -----------------------------------------------------------
    // SOMBRAS
    // -----------------------------------------------------------

    float shadow = ShadowCalculation(
        vPosLightSpace,
        normal,
        lightDir
    );

    vec3 directLight =
        diffuse + specular;

    vec3 finalColor =
        ambient + directLight * (1.0 - shadow * 0.75);

    // -----------------------------------------------------------
    // RESULTADO FINAL
    // -----------------------------------------------------------

    FragColor = vec4(finalColor, base.a);
}