#include <iostream>

#include "SFML/window.h"

int main() {
    try {
        Window window("Lab2", 500, 500, 60);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}

// #include <SFML/Window.hpp>
// #include <GL/glew.h>
// #include <SFML/OpenGL.hpp>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <iostream>
// #include <vector>
// #include <cmath>

// // Функция для загрузки шейдера
// GLuint loadShader(const char* vertexPath, const char* fragmentPath) {
//     // Загрузка и компиляция вершинного шейдера
//     GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//     std::string vertexCode = "#version 330 core\nlayout(location = 0) in vec3 position;\nuniform mat4 model;\nuniform mat4 view;\nuniform mat4 projection;\nvoid main() {\ngl_Position = projection * view * model * vec4(position, 1.0);\n}";
//     const char* vShaderCode = vertexCode.c_str();
//     glShaderSource(vertexShader, 1, &vShaderCode, NULL);
//     glCompileShader(vertexShader);

//     // Загрузка и компиляция фрагментного шейдера
//     GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//     std::string fragmentCode = "#version 330 core\nout vec4 color;\nvoid main() {\ncolor = vec4(0.6, 0.7, 0.8, 1.0);\n}";
//     const char* fShaderCode = fragmentCode.c_str();
//     glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
//     glCompileShader(fragmentShader);

//     // Создание шейдерной программы
//     GLuint shaderProgram = glCreateProgram();
//     glAttachShader(shaderProgram, vertexShader);
//     glAttachShader(shaderProgram, fragmentShader);
//     glLinkProgram(shaderProgram);

//     // Удаление шейдеров после их использования
//     glDeleteShader(vertexShader);
//     glDeleteShader(fragmentShader);

//     return shaderProgram;
// }

// // Функция для создания цилиндра
// void createCylinder(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices, float radius, float height, int segments) {
//     float angleStep = 2.0f * glm::pi<float>() / segments;
//     for (int i = 0; i <= segments; ++i) {
//         float angle = i * angleStep;
//         float x = radius * cos(angle);
//         float z = radius * sin(angle);

//         // Верхняя и нижняя крышки
//         vertices.push_back(x);
//         vertices.push_back(height / 2.0f);
//         vertices.push_back(z);

//         vertices.push_back(x);
//         vertices.push_back(-height / 2.0f);
//         vertices.push_back(z);
//     }

//     for (int i = 0; i < segments; ++i) {
//         indices.push_back(i * 2);
//         indices.push_back(i * 2 + 1);
//         indices.push_back((i * 2 + 2) % (segments * 2));

//         indices.push_back(i * 2 + 1);
//         indices.push_back((i * 2 + 3) % (segments * 2));
//         indices.push_back((i * 2 + 2) % (segments * 2));
//     }
// }

// int main() {
//     sf::Window window(sf::VideoMode(800, 600), "Cylinder with Camera", sf::Style::Default, sf::ContextSettings(24));
//     window.setVerticalSyncEnabled(true);

//     glewExperimental = GL_TRUE;
//     if (glewInit() != GLEW_OK) {
//         std::cerr << "Failed to initialize GLEW" << std::endl;
//         return -1;
//     }

//     std::vector<GLfloat> vertices;
//     std::vector<GLuint> indices;
//     createCylinder(vertices, indices, 1.0f, 2.0f, 36);

//     GLuint VBO, VAO, EBO;
//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//     glGenBuffers(1, &EBO);

//     glBindVertexArray(VAO);

//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//     glEnableVertexAttribArray(0);

//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glBindVertexArray(0);

//     GLuint shaderProgram = loadShader("vertex_shader.glsl", "fragment_shader.glsl");

//     glm::vec3 cameraPos(0.0f, 0.0f, 5.0f);
//     float cameraRadius = 5.0f;
//     float cameraAngle = 0.0f;
//     float cameraHeight = 0.0f;
//     float cameraSpeed = 0.05f;

//     glEnable(GL_DEPTH_TEST);

//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed) {
//                 window.close();
//             }
//         }

//         // Управление камерой
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
//             cameraAngle -= cameraSpeed;
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
//             cameraAngle += cameraSpeed;
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
//             cameraHeight += cameraSpeed;
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
//             cameraHeight -= cameraSpeed;
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
//             cameraRadius -= cameraSpeed;
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
//             cameraRadius += cameraSpeed;
//         }

//         // Ограничения для камеры
//         cameraHeight = glm::clamp(cameraHeight, -2.0f, 2.0f);
//         cameraRadius = glm::clamp(cameraRadius, 3.0f, 10.0f);

//         cameraPos.x = cameraRadius * cos(cameraAngle);
//         cameraPos.z = cameraRadius * sin(cameraAngle);
//         cameraPos.y = cameraHeight;

//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         glm::mat4 model = glm::mat4(1.0f);
//         glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//         glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

//         glUseProgram(shaderProgram);
//         GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
//         GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
//         GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");

//         glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//         glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//         glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

//         glBindVertexArray(VAO);
//         glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//         glBindVertexArray(0);

//         window.display();
//     }

//     glDeleteVertexArrays(1, &VAO);
//     glDeleteBuffers(1, &VBO);
//     glDeleteBuffers(1, &EBO);

//     return 0;
// }