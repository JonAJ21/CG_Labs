// #include <iostream>

// #include "SFML/window.h"

// int main() {
//     try {
//         Window window("Lab5", 500, 500, 60);
//     } catch (const std::runtime_error& e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//         return -1;
//     }
//     return 0;
// }

#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Глобальные переменные для камеры
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse = true;
float mouseSensitivity = 0.1f;
float cameraSpeed = 0.05f;
float deltaTime = 0.0f;

bool cursorGrabbed = true;

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform vec3 cameraPos;
uniform vec3 cameraFront;
uniform float refractionIndex;

struct Ray {
    vec3 origin;
    vec3 direction;
};

struct Hit {
    bool happened;
    vec3 point;
    vec3 normal;
    float t;
    bool isTransparent;
    vec3 color;
};

const vec3 lightPos = vec3(5.0, 5.0, 5.0);
const float ambient = 0.1;
const float AIR_IOR = 1.0;

// Закон Снелля
vec3 snellRefract(vec3 I, vec3 N, float n1, float n2) {
    float eta = n1 / n2;
    float cosI = -dot(N, I);
    float sinT2 = eta * eta * (1.0 - cosI * cosI);
    
    if (sinT2 > 1.0) {
        // Полное внутреннее отражение
        return reflect(I, N);
    }
    
    float cosT = sqrt(1.0 - sinT2);
    return eta * I + (eta * cosI - cosT) * N;
}

// Формула Френеля
float fresnel(vec3 I, vec3 N, float n1, float n2) {
    float cosI = abs(dot(I, N));
    float sinT2 = (n1 * n1 / (n2 * n2)) * (1.0 - cosI * cosI);
    
    if (sinT2 > 1.0) {
        return 1.0; // Полное внутреннее отражение
    }
    
    float cosT = sqrt(1.0 - sinT2);
    float Rs = ((n2 * cosI - n1 * cosT) / (n2 * cosI + n1 * cosT));
    float Rp = ((n1 * cosT - n2 * cosI) / (n1 * cosT + n2 * cosI));
    
    return (Rs * Rs + Rp * Rp) / 2.0;
}

Hit intersectSphere(Ray ray, vec3 center, float radius, vec3 color, bool isTransparent) {
    Hit hit;
    hit.happened = false;
    
    vec3 oc = ray.origin - center;
    float a = dot(ray.direction, ray.direction);
    float b = 2.0 * dot(oc, ray.direction);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4.0 * a * c;
    
    if (discriminant > 0.0) {
        float t = (-b - sqrt(discriminant)) / (2.0 * a);
        if (t > 0.001) {
            hit.happened = true;
            hit.t = t;
            hit.point = ray.origin + t * ray.direction;
            hit.normal = normalize(hit.point - center);
            hit.color = color;
            hit.isTransparent = isTransparent;
        }
    }
    
    return hit;
}

Hit intersectPlane(Ray ray, vec3 normal, float height, vec3 color) {
    Hit hit;
    hit.happened = false;
    
    float denom = dot(normal, ray.direction);
    if (abs(denom) > 0.001) {
        float t = -(dot(ray.origin, normal) + height) / denom;
        if (t > 0.001) {
            hit.happened = true;
            hit.t = t;
            hit.point = ray.origin + t * ray.direction;
            hit.normal = normal;
            hit.color = color;
            hit.isTransparent = false;
            
            // Шахматный узор для пола
            float scale = 1.0;
            bool isWhite = (mod(floor(hit.point.x * scale) + floor(hit.point.z * scale), 2.0) == 0.0);
            hit.color = isWhite ? vec3(0.8) : vec3(0.3);
        }
    }
    
    return hit;
}

Hit intersectScene(Ray ray) {
    Hit hit;
    hit.happened = false;
    hit.t = 1e20;
    
    // Прозрачная сфера
    Hit sphere1 = intersectSphere(ray, vec3(0.0, 0.0, -2.0), 0.5, vec3(0.8, 0.8, 1.0), true);
    if (sphere1.happened && sphere1.t < hit.t) hit = sphere1;
    
    // Непрозрачная сфера
    Hit sphere2 = intersectSphere(ray, vec3(1.0, 0.0, -2.5), 0.5, vec3(1.0, 0.2, 0.2), false);
    if (sphere2.happened && sphere2.t < hit.t) hit = sphere2;
    
    // Пол
    Hit plane = intersectPlane(ray, vec3(0.0, 1.0, 0.0), 1.0, vec3(0.5));
    if (plane.happened && plane.t < hit.t) hit = plane;
    
    return hit;
}

vec3 shade(Hit hit, Ray ray) {
    if (!hit.happened) return vec3(0.1); // Фоновый цвет
    
    vec3 finalColor = vec3(0.0);
    vec3 throughput = vec3(1.0);
    
    for (int bounce = 0; bounce < 4; bounce++) {
        if (!hit.isTransparent) {
            // Для непрозрачных объектов
            vec3 lightDir = normalize(lightPos - hit.point);
            float diff = max(dot(hit.normal, lightDir), 0.0);
            
            // Проверка тени
            Ray shadowRay;
            shadowRay.origin = hit.point + hit.normal * 0.001;
            shadowRay.direction = lightDir;
            Hit shadowHit = intersectScene(shadowRay);
            float shadow = shadowHit.happened ? 0.2 : 1.0;
            
            finalColor += throughput * hit.color * (ambient + diff * shadow);
            break;
        }
        
        float n1 = AIR_IOR;
        float n2 = refractionIndex;
        bool entering = dot(ray.direction, hit.normal) < 0.0;
        vec3 normal = entering ? hit.normal : -hit.normal;
        
        if (!entering) {
            float temp = n1;
            n1 = n2;
            n2 = temp;
        }
        
        float kr = fresnel(ray.direction, normal, n1, n2);
        
        // Преломление
        if (kr < 0.999) {
            Ray refractRay;
            refractRay.origin = hit.point - normal * 0.001;
            refractRay.direction = snellRefract(ray.direction, normal, n1, n2);
            hit = intersectScene(refractRay);
            
            if (!hit.happened) {
                finalColor += throughput * (1.0 - kr) * vec3(0.1);
                break;
            }
            
            throughput *= (1.0 - kr) * hit.color;
            ray = refractRay;
            continue;
        }
        
        // Отражение
        Ray reflectRay;
        reflectRay.origin = hit.point + normal * 0.001;
        reflectRay.direction = reflect(ray.direction, normal);
        hit = intersectScene(reflectRay);
        
        if (!hit.happened) {
            finalColor += throughput * kr * vec3(0.1);
            break;
        }
        
        throughput *= kr * hit.color;
        ray = reflectRay;
        
        // Прекращаем трассировку, если вклад стал слишком маленьким
        if (length(throughput) < 0.01) break;
    }
    
    return finalColor;
}

void main() {
    vec2 uv = TexCoord * 2.0 - 1.0;
    
    vec3 right = normalize(cross(cameraFront, vec3(0.0, 1.0, 0.0)));
    vec3 up = normalize(cross(right, cameraFront));
    
    Ray ray;
    ray.origin = cameraPos;
    ray.direction = normalize(cameraFront + uv.x * right + uv.y * up);
    
    Hit hit = intersectScene(ray);
    vec3 color = shade(hit, ray);
    
    // Гамма-коррекция
    color = pow(color, vec3(1.0/2.2));
    
    FragColor = vec4(color, 1.0);
}
)";

void processMouseMovement(float xpos, float ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = mouseSensitivity;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void checkShaderCompileErrors(GLuint shader, const char* type) {
    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
    }
}

int main() {
    // Создание окна SFML
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window(sf::VideoMode(800, 600), "Refraction Demo", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);
    window.setMouseCursorGrabbed(true);

    // Инициализация GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Вывод информации об OpenGL
    std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Компиляция шейдеров
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkShaderCompileErrors(vertexShader, "VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompileErrors(fragmentShader, "FRAGMENT");

    // Создание шейдерной программы
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Проверка линковки
    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Создание VAO и VBO
    float vertices[] = {
        // positions        // texture coords
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Установка начальных uniform-переменных
    glUseProgram(shaderProgram);
    float refractionIndex = 1.5f; // Начальное значение (стекло)
    glUniform1f(glGetUniformLocation(shaderProgram, "refractionIndex"), refractionIndex);

    // Часы для измерения времени между кадрами
    sf::Clock clock;

    // Главный цикл
    while (window.isOpen()) {
        // Обновление deltaTime
        deltaTime = clock.restart().asSeconds();
        cameraSpeed = 2.5f * deltaTime;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseMoved) {
                processMouseMovement(event.mouseMove.x, event.mouseMove.y);
            }
            else if (event.type == sf::Event::KeyPressed) {
                // if (event.key.code == sf::Keyboard::Escape)
                //     window.close();
                if (event.key.code == sf::Keyboard::Tab) {
                    cursorGrabbed = !cursorGrabbed;
                    window.setMouseCursorGrabbed(cursorGrabbed);
                    window.setMouseCursorVisible(!cursorGrabbed);
                    firstMouse = true;
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    refractionIndex += 0.1f;
                    glUniform1f(glGetUniformLocation(shaderProgram, "refractionIndex"), refractionIndex);
                    std::cout << "Refraction index: " << refractionIndex << std::endl;
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    refractionIndex = std::max(1.0f, refractionIndex - 0.1f);
                    glUniform1f(glGetUniformLocation(shaderProgram, "refractionIndex"), refractionIndex);
                    std::cout << "Refraction index: " << refractionIndex << std::endl;
                }
                else if (event.key.code == sf::Keyboard::W && event.key.control) { // Ctrl+W для воды
                    refractionIndex = 1.33f;
                    glUniform1f(glGetUniformLocation(shaderProgram, "refractionIndex"), refractionIndex);
                    std::cout << "Water: " << refractionIndex << std::endl;
                }
                else if (event.key.code == sf::Keyboard::G && event.key.control) { // G для стекла
                    refractionIndex = 1.5f;
                    glUniform1f(glGetUniformLocation(shaderProgram, "refractionIndex"), refractionIndex);
                    std::cout << "Glass: " << refractionIndex << std::endl;
                }
                else if (event.key.code == sf::Keyboard::D && event.key.control) { // D для алмаза
                    refractionIndex = 2.42f;
                    glUniform1f(glGetUniformLocation(shaderProgram, "refractionIndex"), refractionIndex);
                    std::cout << "Diamond: " << refractionIndex << std::endl;
                }
            }
        }

        // Обработка движения камеры
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            cameraPos += cameraSpeed * cameraFront;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            cameraPos -= cameraSpeed * cameraFront;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            cameraPos += cameraSpeed * cameraUp;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            cameraPos -= cameraSpeed * cameraUp;

        // Обновление uniform-переменных камеры
        glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1, glm::value_ptr(cameraPos));
        glUniform3fv(glGetUniformLocation(shaderProgram, "cameraFront"), 1, glm::value_ptr(cameraFront));

        // Рендеринг
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        window.display();
    }

    // Очистка
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    return 0;
}