#include <iostream>

#include "SFML/window.h"

int main() {
    try {
        Window window("Lab5", 500, 500, 60);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}

// #include <GL/glew.h>
// #include <SFML/Window.hpp>
// #include <SFML/OpenGL.hpp>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

// // Вершинный шейдер
// const char* vertexShaderSource = R"(
//     #version 330 core
//     layout (location = 0) in vec3 aPos;
//     layout (location = 1) in vec3 aNormal;
    
//     uniform mat4 model;
//     uniform mat4 view;
//     uniform mat4 projection;
    
//     out vec3 Normal;
//     out vec3 FragPos;
    
//     void main() {
//         FragPos = vec3(model * vec4(aPos, 1.0));
//         Normal = mat3(transpose(inverse(model))) * aNormal;
//         gl_Position = projection * view * model * vec4(aPos, 1.0);
//     }
// )";

// // Фрагментный шейдер
// const char* fragmentShaderSource = R"(
//     #version 330 core
//     out vec4 FragColor;
    
//     in vec3 Normal;
//     in vec3 FragPos;
    
//     uniform vec3 lightPos;
//     uniform vec3 lightColor;
    
//     void main() {
//         vec3 norm = normalize(Normal);
//         vec3 lightDir = normalize(lightPos - FragPos);
        
//         float diff = max(dot(norm, lightDir), 0.0);
//         vec3 diffuse = diff * lightColor;
        
//         vec3 result = diffuse * vec3(1.0, 0.5, 0.2);
//         FragColor = vec4(result, 1.0);
//     }
// )";

// int main() {
//     sf::Window window(sf::VideoMode(800, 600), "Пирамида с нормалями", sf::Style::Default, sf::ContextSettings(24));
//     window.setVerticalSyncEnabled(true);
    
//     glewInit();
    
//     // Вершины пирамиды и их нормали
//     float vertices[] = {
//         // Основание
//         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//          0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//         -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        
//         // Передняя грань
//         -0.5f, -0.5f,  0.5f,  0.0f,  0.5f,  0.5f,
//          0.5f, -0.5f,  0.5f,  0.0f,  0.5f,  0.5f,
//          0.0f,  0.5f,  0.0f,  0.0f,  0.5f,  0.5f,
        
//         // Правая грань
//          0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.0f,
//          0.5f, -0.5f, -0.5f,  0.5f,  0.5f,  0.0f,
//          0.0f,  0.5f,  0.0f,  0.5f,  0.5f,  0.0f,
        
//         // Задняя грань
//          0.5f, -0.5f, -0.5f,  0.0f,  0.5f, -0.5f,
//         -0.5f, -0.5f, -0.5f,  0.0f,  0.5f, -0.5f,
//          0.0f,  0.5f,  0.0f,  0.0f,  0.5f, -0.5f,
        
//         // Левая грань
//         -0.5f, -0.5f, -0.5f, -0.5f,  0.5f,  0.0f,
//         -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.0f,
//          0.0f,  0.5f,  0.0f, -0.5f,  0.5f,  0.0f
//     };
    
//     unsigned int indices[] = {
//         0, 1, 2, 2, 3, 0,  // Основание
//         4, 5, 6,           // Передняя грань
//         7, 8, 9,           // Правая грань
//         10, 11, 12,        // Задняя грань
//         13, 14, 15         // Левая грань
//     };

//     // Создание и компиляция шейдеров
//     unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//     glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//     glCompileShader(vertexShader);

//     unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//     glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//     glCompileShader(fragmentShader);

//     unsigned int shaderProgram = glCreateProgram();
//     glAttachShader(shaderProgram, vertexShader);
//     glAttachShader(shaderProgram, fragmentShader);
//     glLinkProgram(shaderProgram);

//     glDeleteShader(vertexShader);
//     glDeleteShader(fragmentShader);

//     // Создание буферов
//     unsigned int VBO, VAO, EBO;
//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//     glGenBuffers(1, &EBO);

//     glBindVertexArray(VAO);

//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

//     // Настройка атрибутов вершин
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//     glEnableVertexAttribArray(1);

//     // Настройка OpenGL
//     glEnable(GL_DEPTH_TEST);
    
//     // Матрицы для преобразований
//     glm::mat4 model = glm::mat4(1.0f);
//     glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f),
//                                 glm::vec3(0.0f, 0.0f, 0.0f),
//                                 glm::vec3(0.0f, 1.0f, 0.0f));
//     glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

//     // Основной цикл
//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//         }

//         glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         glUseProgram(shaderProgram);

//         // Обновление юниформ-переменных
//         model = glm::rotate(model, glm::radians(0.005f), glm::vec3(0.0f, 1.0f, 0.0f));
        
//         glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
//         glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
//         glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
//         glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), 2.0f, 2.0f, 2.0f);
//         glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);

//         // Отрисовка пирамиды
//         glBindVertexArray(VAO);
//         glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

//         window.display();
//     }

//     // Очистка ресурсов
//     glDeleteVertexArrays(1, &VAO);
//     glDeleteBuffers(1, &VBO);
//     glDeleteBuffers(1, &EBO);
//     glDeleteProgram(shaderProgram);

//     return 0;
// }

// #include <GL/glew.h>
// #include <SFML/Window.hpp>
// #include <SFML/OpenGL.hpp>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <vector>
// #include <cmath>

// // Вершинный шейдер
// const char* vertexShaderSource = R"(
//     #version 330 core
//     layout (location = 0) in vec3 aPos;
//     layout (location = 1) in vec3 aNormal;
    
//     uniform mat4 model;
//     uniform mat4 view;
//     uniform mat4 projection;
    
//     out vec3 Normal;
//     out vec3 Position;
    
//     void main() {
//         Normal = mat3(transpose(inverse(model))) * aNormal;
//         Position = vec3(model * vec4(aPos, 1.0));
//         gl_Position = projection * view * model * vec4(aPos, 1.0);
//     }
// )";

// // Фрагментный шейдер для прозрачной сферы
// const char* transparentFragmentShaderSource = R"(
//     #version 330 core
//     out vec4 FragColor;
    
//     in vec3 Normal;
//     in vec3 Position;
    
//     uniform vec3 cameraPos;
//     uniform float refractionIndex;
//     uniform samplerCube skybox;
    
//     void main() {
//         vec3 I = normalize(Position - cameraPos);
//         vec3 N = normalize(Normal);
        
//         float ratio = 1.00 / refractionIndex;
//         vec3 R = refract(I, N, ratio);
        
//         FragColor = vec4(texture(skybox, R).rgb, 0.5);
//     }
// )";

// // Фрагментный шейдер для непрозрачной сферы
// const char* opaqueFragmentShaderSource = R"(
//     #version 330 core
//     out vec4 FragColor;
    
//     in vec3 Normal;
//     in vec3 Position;
    
//     uniform vec3 lightPos;
//     uniform vec3 lightColor;
//     uniform vec3 objectColor;
    
//     void main() {
//         vec3 norm = normalize(Normal);
//         vec3 lightDir = normalize(lightPos - Position);
        
//         float diff = max(dot(norm, lightDir), 0.0);
//         vec3 diffuse = diff * lightColor;
        
//         vec3 result = diffuse * objectColor;
//         FragColor = vec4(result, 1.0);
//     }
// )";

// // Функция для создания сферы
// std::vector<float> createSphere(float radius, int sectors, int stacks) {
//     std::vector<float> vertices;
    
//     for(int i = 0; i <= stacks; ++i) {
//         float phi = M_PI * float(i) / float(stacks);
//         for(int j = 0; j <= sectors; ++j) {
//             float theta = 2.0f * M_PI * float(j) / float(sectors);
            
//             float x = radius * sin(phi) * cos(theta);
//             float y = radius * cos(phi);
//             float z = radius * sin(phi) * sin(theta);
            
//             // Позиция
//             vertices.push_back(x);
//             vertices.push_back(y);
//             vertices.push_back(z);
            
//             // Нормаль
//             vertices.push_back(x/radius);
//             vertices.push_back(y/radius);
//             vertices.push_back(z/radius);
//         }
//     }
    
//     return vertices;
// }

// int main() {
//     sf::Window window(sf::VideoMode(800, 600), "Преломление и прозрачность", sf::Style::Default, sf::ContextSettings(24));
//     window.setVerticalSyncEnabled(true);
    
//     glewInit();
    
//     // Создание сфер
//     std::vector<float> sphereVertices = createSphere(0.5f, 32, 16);
    
//     // Создание и компиляция шейдеров
//     unsigned int transparentShaderProgram = glCreateProgram();
//     unsigned int opaqueShaderProgram = glCreateProgram();
    
//     // ... Компиляция шейдеров (аналогично предыдущему примеру)
    
//     // Создание буферов для сфер
//     unsigned int VAO[2], VBO[2];
//     glGenVertexArrays(2, VAO);
//     glGenBuffers(2, VBO);
    
//     for(int i = 0; i < 2; ++i) {
//         glBindVertexArray(VAO[i]);
//         glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
//         glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW);
        
//         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//         glEnableVertexAttribArray(0);
//         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//         glEnableVertexAttribArray(1);
//     }
    
//     // Настройка OpenGL
//     glEnable(GL_DEPTH_TEST);
//     glEnable(GL_BLEND);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
//     // Параметры сцены
//     float refractionIndex = 1.33f; // Начальное значение (вода)
//     glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
    
//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//             else if (event.type == sf::Event::KeyPressed) {
//                 // Управление коэффициентом преломления
//                 if (event.key.code == sf::Keyboard::Up)
//                     refractionIndex = std::min(refractionIndex + 0.1f, 2.42f);
//                 if (event.key.code == sf::Keyboard::Down)
//                     refractionIndex = std::max(refractionIndex - 0.1f, 1.0f);
//             }
//         }
        
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
//         // Матрицы преобразования
//         glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//         glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);
        
//         // Отрисовка непрозрачной сферы
//         glUseProgram(opaqueShaderProgram);
//         glm::mat4 model1 = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        
//         glUniformMatrix4fv(glGetUniformLocation(opaqueShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model1));
//         glUniformMatrix4fv(glGetUniformLocation(opaqueShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
//         glUniformMatrix4fv(glGetUniformLocation(opaqueShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//         glUniform3f(glGetUniformLocation(opaqueShaderProgram, "objectColor"), 0.7f, 0.2f, 0.2f);
//         glUniform3f(glGetUniformLocation(opaqueShaderProgram, "lightPos"), 2.0f, 2.0f, 2.0f);
//         glUniform3f(glGetUniformLocation(opaqueShaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
        
//         glBindVertexArray(VAO[0]);
//         glDrawArrays(GL_TRIANGLE_STRIP, 0, sphereVertices.size()/6);
        
//         // Отрисовка прозрачной сферы
//         glUseProgram(transparentShaderProgram);
//         glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        
//         glUniformMatrix4fv(glGetUniformLocation(transparentShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model2));
//         glUniformMatrix4fv(glGetUniformLocation(transparentShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
//         glUniformMatrix4fv(glGetUniformLocation(transparentShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//         glUniform3fv(glGetUniformLocation(transparentShaderProgram, "cameraPos"), 1, glm::value_ptr(cameraPos));
//         glUniform1f(glGetUniformLocation(transparentShaderProgram, "refractionIndex"), refractionIndex);
        
//         glBindVertexArray(VAO[1]);
//         glDrawArrays(GL_TRIANGLE_STRIP, 0, sphereVertices.size()/6);
        
//         window.display();
//     }
    
//     // Очистка ресурсов
//     glDeleteVertexArrays(2, VAO);
//     glDeleteBuffers(2, VBO);
//     glDeleteProgram(transparentShaderProgram);
//     glDeleteProgram(opaqueShaderProgram);
    
//     return 0;
// }