#version 450

out vec4 color;

in vec3 worldPosition;
in vec3 screenPosition;
in vec3 fColor;
in vec3 fNormal;

in vec3 fSpotlightPosition;
in vec3 fSpotlightDirection;
in float fSpotlightCutoff;
in float fSpotlightOuterCutoff;

void main() {

    // Вычисляем направление от фрагмента к прожектору
    vec3 fragToLight = normalize(fSpotlightPosition - worldPosition);
    
    // Вычисляем угол между направлением прожектора и направлением к фрагменту
    float theta = dot(fragToLight, normalize(-fSpotlightDirection));

    float ambient_coef = 0.1;
    float diffuse_coef = 1.0;
    float specular_coef = 1.0;
    float shininess = 32.0;

    float constant = 1.0;
    float linear = 0.09;
    float quadratic = 0.032;

    // if (theta > fSpotlightCutoff) {
        // Базовое фоновое освещение
        vec3 ambient = ambient_coef * fColor;
        // Диффузное освещение
        vec3 normal = normalize(fNormal);
        float diff = max(dot(normal, fragToLight), 0.0);
        vec3 diffuse = diffuse_coef * diff * fColor;

        // Отраженное освещение
        vec3 viewDir = normalize(screenPosition - worldPosition);
        vec3 reflectDir = reflect(-fragToLight, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = specular_coef * spec * fColor;

        // Затухание
        float epsilon = fSpotlightCutoff - fSpotlightOuterCutoff;
        float intensity = clamp((theta - fSpotlightOuterCutoff) / epsilon, 0.0, 1.0);
        diffuse *= intensity;
        specular *= intensity;
        
        float distance = length(fSpotlightPosition - worldPosition);
        float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);
        
        // Умножаем на затухание
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        vec3 result = ambient + diffuse + specular;
        color = vec4(result, 1.0);
    // } else {
    //     color = vec4(ambient_coef * fColor, 1.0);
    // }
    



        
    // // Диффузное освещение
    // float diff = max(dot(normalize(fNormal), fragToLight), 0.0);
    // vec3 diffuse = diff * fColor;
    
    // // Вычисляем интенсивность освещения с плавным переходом
    // float epsilon = fSpotlightCutoff - fSpotlightOuterCutoff;
    // float intensity = clamp((theta - fSpotlightOuterCutoff) / epsilon, 0.0, 1.0);
    
    // if (theta > fSpotlightCutoff) {
    //     color = vec4(fColor, 1.0);
    // } else {
    //     color = vec4(ambient + diffuse, 1.0);
    // }


    //  // Итоговый цвет с учетом интенсивности прожектора
    // vec3 result = ambient + (diffuse * intensity);
    // color = vec4(result, 1.0);

    


}