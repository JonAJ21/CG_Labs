#version 450

in vec3 position;
in vec3 color;
in vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 worldPosition;
out vec3 screenPosition;
out vec3 fNormal;
out vec3 fColor;

void main() {
    // Преобразование позиции в мировые координаты
    worldPosition = vec3(modelMatrix * vec4(position, 1.0));
    
    // Преобразование позиции в экранные координаты
    vec4 viewPos = viewMatrix * modelMatrix * vec4(position, 1.0);
    screenPosition = viewPos.xyz;
    
    // Преобразование нормали в мировое пространство
    fNormal = normalize(mat3(modelMatrix) * normal);
    
    // Передача цвета во фрагментный шейдер
    fColor = color;

    // Итоговая позиция вершины
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
    
}