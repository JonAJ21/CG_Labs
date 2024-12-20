#version 450

out vec4 color;

in vec3 worldPosition;
in vec3 screenPosition;
in vec3 fColor;
in vec3 fNormal;



// in vec3 fSpotlightPosition[NR_POINT_LIGHTS];
// in vec3 fSpotlightDirection[NR_POINT_LIGHTS];
// in float fSpotlightCutoff[NR_POINT_LIGHTS];
// in float fSpotlightOuterCutoff[NR_POINT_LIGHTS];

#define NR_POINT_LIGHTS 1
    
uniform vec3 fSpotlightPosition[NR_POINT_LIGHTS];
uniform vec3 fSpotlightDirection[NR_POINT_LIGHTS];
uniform float fSpotlightCutoff[NR_POINT_LIGHTS];
uniform float fSpotlightOuterCutoff[NR_POINT_LIGHTS];

vec3 calcColor(vec3 spotlightPosition, vec3 spotlightDirection, float spotlightCutoff, float spotlightOuterCutoff);

void main() {

    vec3 result = vec3(0.0);

    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
        //result += calcColor(fSpotlightPosition, fSpotlightDirection, fSpotlightCutoff, fSpotlightOuterCutoff);
        result += calcColor(fSpotlightPosition[i], fSpotlightDirection[i], fSpotlightCutoff[i], fSpotlightOuterCutoff[i]);
    }
    
    vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);

    color = vec4(result, 1.0) * lightColor;
}

vec3 calcColor(vec3 spotlightPosition, vec3 spotlightDirection, float spotlightCutoff, float spotlightOuterCutoff) {
    // Вычисляем направление от фрагмента к прожектору
    vec3 fragToLight = normalize(spotlightPosition - worldPosition);
    
    // Вычисляем угол между направлением прожектора и направлением к фрагменту
    float theta = dot(fragToLight, normalize(-spotlightDirection));

    // float ambient_coef = 0.1;
    // float diffuse_coef = 1.0;
    // float specular_coef = 1.0;

    vec3 ambient = vec4(0.2125, 0.1275, 0.054, 1.0).xyz;
    vec3 diffuse = vec4(0.714, 0.4284, 0.18144, 1.0).xyz;
    vec3 specular = vec4(0.393548, 0.271906, 0.166721, 1.0).xyz;
    
    float shininess = 76.8;

    float constant = 1.0;
    float linear = 0.09;
    float quadratic = 0.032;

    // if (theta > fSpotlightCutoff) {
    // Базовое фоновое освещение
    //ambient *= fColor;
    // Диффузное освещение
    vec3 normal = normalize(fNormal);
    float diff = max(dot(normal, fragToLight), 0.0);
    diffuse *= diff;// * fColor;

    // Отраженное освещение
    vec3 viewDir = normalize(screenPosition - worldPosition);
    vec3 reflectDir = reflect(-fragToLight, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    specular *= spec;// * fColor;

    // Затухание
    float epsilon = spotlightCutoff - spotlightOuterCutoff;
    float intensity = clamp((theta - spotlightOuterCutoff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;
        
    float distance = length(spotlightPosition - worldPosition);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);
        
    //Умножаем на затухание
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    return result;
    // return vec4(result, 1.0);
}