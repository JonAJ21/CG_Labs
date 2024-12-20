#version 450

out vec4 color;

in vec3 worldPosition;
in vec3 screenPosition;
in vec3 fColor;
in vec3 fNormal;

#define NR_POINT_LIGHTS 1
    
uniform vec3 fSpotlightPosition[NR_POINT_LIGHTS];
uniform vec3 fSpotlightDirection[NR_POINT_LIGHTS];
uniform float fSpotlightCutoff[NR_POINT_LIGHTS];
uniform float fSpotlightOuterCutoff[NR_POINT_LIGHTS];

vec3 calcColor(vec3 spotlightPosition, vec3 spotlightDirection, float spotlightCutoff, float spotlightOuterCutoff);
float calcShadow(vec3 fragPos, vec3 lightPos, vec3 normal, vec3 lightDir);
vec3 calcRefraction(vec3 incident, vec3 normal, float n1, float n2);

void main() {
    vec3 result = vec3(0.0);

    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += calcColor(fSpotlightPosition[i], fSpotlightDirection[i], fSpotlightCutoff[i], fSpotlightOuterCutoff[i]);
    }
    
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
    
    color = vec4(result, 1.0) * lightColor;
}

float calcShadow(vec3 fragPos, vec3 lightPos, vec3 normal, vec3 lightDir) {
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = 0.0;
    
    if(dot(normal, lightDir) < bias) {
        shadow = 0.5;
    }
    
    return shadow;
}

vec3 calcRefraction(vec3 incident, vec3 normal, float n1, float n2) {
    float n = n1 / n2;
    float cosI = -dot(normal, incident);
    float sinT2 = n * n * (1.0 - cosI * cosI);
    
    if (sinT2 > 1.0) {
        return reflect(incident, normal); // Полное внутреннее отражение
    }
    
    float cosT = sqrt(1.0 - sinT2);
    return n * incident + (n * cosI - cosT) * normal;
}

vec3 calcColor(vec3 spotlightPosition, vec3 spotlightDirection, float spotlightCutoff, float spotlightOuterCutoff) {
    vec3 fragToLight = normalize(spotlightPosition - worldPosition);
    float theta = dot(fragToLight, normalize(-spotlightDirection));

    // vec3 ambient = vec4(0.2125, 0.1275, 0.054, 1.0).xyz;
    // vec3 diffuse = vec4(0.714, 0.4284, 0.18144, 1.0).xyz;
    // vec3 specular = vec4(0.393548, 0.271906, 0.166721, 1.0).xyz;

     // Для воды
    vec3 ambient = vec4(0.0, 0.1, 0.2, 1.0).xyz;
    vec3 diffuse = vec4(0.0, 0.2, 0.4, 1.0).xyz;
    vec3 specular = vec4(0.8, 0.8, 0.8, 1.0).xyz;
    float shininess = 50.0;
    float n2 = 1.33; // Коэффициент преломления воды

    
    //float shininess = 76.8;
    float n1 = 1.0; // Коэффициент преломления воздуха
    //float n2 = 1.5; // Коэффициент преломления материала

    float constant = 1.0;
    float linear = 0.09;
    float quadratic = 0.032;

    vec3 normal = normalize(fNormal);
    float diff = max(dot(normal, fragToLight), 0.0);
    diffuse *= diff;

    vec3 viewDir = normalize(screenPosition - worldPosition);
    
    // Вычисляем преломленный луч
    vec3 refractedDir = calcRefraction(-viewDir, normal, n1, n2);
    vec3 reflectDir = reflect(-fragToLight, normal);
    
    // Учитываем как отражение, так и преломление
    float spec = pow(max(dot(refractedDir, reflectDir), 0.0), shininess);
    specular *= spec;

    float epsilon = spotlightCutoff - spotlightOuterCutoff;
    float intensity = clamp((theta - spotlightOuterCutoff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;
        
    float distance = length(spotlightPosition - worldPosition);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);
        
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;


    // Вычисляем тень
    float shadow = calcShadow(worldPosition, spotlightPosition, normal, fragToLight);
    
    // Применяем тень к диффузной и отраженной составляющим
    vec3 result = ambient + (1.0 - shadow) * (diffuse + specular);
    return result;
}