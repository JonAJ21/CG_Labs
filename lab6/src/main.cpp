// #include <iostream>

// #include "SFML/window.h"

// int main() {
//     try {
//         Window window("Lab6", 500, 500, 60);
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
#include <string>
#include <memory>

// Шейдерные исходники
const char* vertexShaderSource = R"(
#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec3 worldPosition;
out vec3 screenPosition;
out vec2 texCoord;

uniform vec3 cameraPos;
uniform vec3 cameraFront;

void main() {
    gl_Position = vec4(aPos, 1.0);
    worldPosition = aPos;
    
    // Создаем базовый вектор направления луча
    vec3 rayDir = normalize(vec3(aPos.x, aPos.y, -1.0));
    
    // Создаем правый и верхний векторы из направления камеры
    vec3 right = normalize(cross(cameraFront, vec3(0.0, 1.0, 0.0)));
    vec3 up = normalize(cross(right, cameraFront));
    
    // Создаем матрицу поворота из базисных векторов камеры
    mat3 viewMatrix = mat3(
        right,
        up,
        -cameraFront
    );
    
    // Поворачиваем луч в соответствии с ориентацией камеры
    screenPosition = viewMatrix * rayDir;
    texCoord = aTexCoord;
}
)";

const char* fragmentShaderSource = R"(
#version 450
out vec4 color;
in vec3 worldPosition;
in vec3 screenPosition;
in vec2 texCoord;

uniform vec3 cameraPos;
uniform vec3 cameraFront;
uniform float refractionIndex;

#define AIR_IOR 1.0

struct Ray {
    vec3 origin;
    vec3 direction;
};

struct Hit {
    bool happened;
    float t;
    vec3 point;
    vec3 normal;
    vec3 color;
    bool isTransparent;
};

vec3 lightPos = vec3(2.0, 2.0, 2.0);
float ambient = 0.1;

float fresnel(vec3 I, vec3 N, float n1, float n2) {
    float cosI = abs(dot(I, N));
    float sinT2 = (n1 * n1 / (n2 * n2)) * (1.0 - cosI * cosI);
    
    if (sinT2 > 1.0) {
        return 1.0; // Полное внутреннее отражение
    }
    
    float cosT = sqrt(1.0 - sinT2);
    float Rs = ((n2 * cosI - n1 * cosT) / (n2 * cosI + n1 * cosT));
    float Rp = ((n1 * cosT - n2 * cosI) / (n1 * cosT + n2 * cosI));
    
    return (Rs * Rs + Rp * Rp) / 2;
}

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

Hit intersectPlane(Ray ray, vec3 normal, float height, vec3 color) {
    Hit hit;
    hit.happened = false;
    
    float denom = dot(normal, ray.direction);
    if (abs(denom) > 0.0001) {
        vec3 p0 = vec3(0.0, -height, 0.0);
        float t = dot(p0 - ray.origin, normal) / denom;
        if (t > 0.0001) {
            hit.happened = true;
            hit.t = t;
            hit.point = ray.origin + t * ray.direction;
            hit.normal = normal;
            hit.color = color;
            hit.isTransparent = false;
        }
    }
    
    return hit;
}

Hit intersectSphere(Ray ray, vec3 center, float radius, vec3 color, bool isTransparent) {
    Hit hit;
    hit.happened = false;
    
    vec3 oc = ray.origin - center;
    float a = dot(ray.direction, ray.direction);
    float b = 2.0 * dot(oc, ray.direction);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    
    if (discriminant > 0.0) {
        float t = (-b - sqrt(discriminant)) / (2.0 * a);
        if (t > 0.0001) {
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

Hit intersectCylinder(Ray ray, vec3 base, vec3 axis, float radius, float height, vec3 color, bool isTransparent) {
    Hit hit;
    hit.happened = false;
    
    vec3 rc = ray.origin - base;
    vec3 n = normalize(axis);
    float d = dot(ray.direction, n);
    vec3 O = rc - dot(rc, n) * n;
    vec3 D = ray.direction - d * n;
    
    float a = dot(D, D);
    float b = 2.0 * dot(O, D);
    float c = dot(O, O) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    
    if (discriminant > 0.0) {
        float t = (-b - sqrt(discriminant)) / (2.0 * a);
        if (t > 0.0001) {
            vec3 p = ray.origin + t * ray.direction;
            float h = dot(p - base, n);
            
            if (h >= 0.0 && h <= height) {
                hit.happened = true;
                hit.t = t;
                hit.point = p;
                hit.normal = normalize(p - (base + h * n));
                hit.color = color;
                hit.isTransparent = isTransparent;
            }
        }
    }
    
    return hit;
}

Hit intersectCone(Ray ray, vec3 apex, vec3 axis, float angle, float height, vec3 color, bool isTransparent) {
    Hit hit;
    hit.happened = false;
    
    vec3 co = ray.origin - apex;
    vec3 n = normalize(axis);
    float cosa = cos(angle);
    float cosa2 = cosa * cosa;
    
    float dv = dot(ray.direction, n);
    float cv = dot(co, n);
    
    float a = dv * dv - cosa2;
    float b = 2.0 * (dv * cv - dot(ray.direction, co) * cosa2);
    float c = cv * cv - dot(co, co) * cosa2;
    
    float discriminant = b * b - 4 * a * c;
    
    if (discriminant > 0.0) {
        float t = (-b - sqrt(discriminant)) / (2.0 * a);
        if (t > 0.0001) {
            vec3 p = ray.origin + t * ray.direction;
            float h = dot(p - apex, n);
            
            if (h >= 0.0 && h <= height) {
                hit.happened = true;
                hit.t = t;
                hit.point = p;
                vec3 ph = apex + h * n;
                hit.normal = normalize(hit.point - ph);
                hit.color = color;
                hit.isTransparent = isTransparent;
            }
        }
    }
    
    return hit;
}

Hit intersectScene(Ray ray) {
    Hit hit;
    hit.happened = false;
    hit.t = 1e20;
    
    Hit sphere1 = intersectSphere(ray, vec3(0.0, 0.0, -2.0), 0.5, vec3(0.8, 0.8, 1.0), true);
    if (sphere1.happened && sphere1.t < hit.t) hit = sphere1;
    
    Hit sphere2 = intersectSphere(ray, vec3(1.0, 0.0, -2.5), 0.5, vec3(1.0, 0.2, 0.2), false);
    if (sphere2.happened && sphere2.t < hit.t) hit = sphere2;
    
    Hit cylinder = intersectCylinder(ray, vec3(-1.5, -1.0, -2.5), vec3(0.0, 1.0, 0.0), 0.3, 1.0, vec3(0.2, 0.8, 0.2), false);
    if (cylinder.happened && cylinder.t < hit.t) hit = cylinder;
    
    Hit cone = intersectCone(ray, vec3(1.5, -1.0, -2.5), vec3(0.0, 1.0, 0.0), 0.5, 1.0, vec3(0.8, 0.4, 0.0), false);
    if (cone.happened && cone.t < hit.t) hit = cone;
    
    Hit plane = intersectPlane(ray, vec3(0.0, 1.0, 0.0), 1.0, vec3(0.5));
    if (plane.happened && plane.t < hit.t) hit = plane;
    
    return hit;
}

vec3 shade(Hit hit, Ray ray) {
    if (!hit.happened) return vec3(0.1);
    
    vec3 finalColor = vec3(0.0);
    vec3 throughput = vec3(1.0);
    
    for (int bounce = 0; bounce < 4; bounce++) {
        if (!hit.isTransparent) {
            vec3 lightDir = normalize(lightPos - hit.point);
            float diff = max(dot(hit.normal, lightDir), 0.0);
            
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
    }
    
    return finalColor;
}

void main() {
    vec3 rayOrigin = cameraPos;
    vec3 rayDirection = normalize(screenPosition);
    
    Ray ray;
    ray.origin = rayOrigin;
    ray.direction = rayDirection;
    
    Hit hit = intersectScene(ray);
    vec3 result = shade(hit, ray);
    
    color = vec4(result, 1.0);
}
)";

class Camera {
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    float yaw;
    float pitch;
    float lastX;
    float lastY;
    bool firstMouse;
    float sensitivity;
    float speed;
    float deltaTime;

public:
    Camera() : 
        position(glm::vec3(0.0f, 0.0f, 3.0f)),
        front(glm::vec3(0.0f, 0.0f, -1.0f)),
        up(glm::vec3(0.0f, 1.0f, 0.0f)),
        right(glm::vec3(1.0f, 0.0f, 0.0f)),
        yaw(-90.0f),
        pitch(0.0f),
        lastX(400.0f),
        lastY(300.0f),
        firstMouse(true),
        sensitivity(0.1f),
        speed(0.05f),
        deltaTime(0.0f)
    {
        updateCameraVectors();
    }

    void processMouseMovement(float xpos, float ypos) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // Инвертируем это значение
        lastX = xpos;
        lastY = ypos;

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        updateCameraVectors();
    }

    void updateDeltaTime(float dt) {
        deltaTime = dt;
        speed = 2.5f * deltaTime;
    }

    void moveForward() { position += speed * front; }
    void moveBackward() { position -= speed * front; }
    void moveLeft() { position -= speed * right; }
    void moveRight() { position += speed * right; }
    void moveUp() { position += speed * up; }
    void moveDown() { position -= speed * up; }

    glm::vec3 getPosition() const { return position; }
    glm::vec3 getFront() const { return front; }
    void resetFirstMouse() { firstMouse = true; }

private:
    void updateCameraVectors() {
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);
        
        right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
        up = glm::normalize(glm::cross(right, front));
    }
};

class Shader {
private:
    GLuint program;

public:
    Shader(const char* vertSrc, const char* fragSrc) {
        compile(vertSrc, fragSrc);
    }

    ~Shader() {
        glDeleteProgram(program);
    }

    void use() { glUseProgram(program); }
    GLuint getProgram() const { return program; }

    void setFloat(const char* name, float value) {
        glUniform1f(glGetUniformLocation(program, name), value);
    }

    void setVec3(const char* name, const glm::vec3& value) {
        glUniform3fv(glGetUniformLocation(program, name), 1, glm::value_ptr(value));
    }

private:
    void compile(const char* vertexSource, const char* fragmentSource) {
        GLuint vertex = compileShader(GL_VERTEX_SHADER, vertexSource);
        GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

        program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);

        checkLinkErrors();

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    GLuint compileShader(GLenum type, const char* source) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);
        checkCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
        return shader;
    }

    void checkCompileErrors(GLuint shader, const char* type) {
        GLint success;
        GLchar infoLog[1024];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
            throw std::runtime_error("Shader compilation failed");
        }
    }

    void checkLinkErrors() {
        GLint success;
        GLchar infoLog[1024];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << std::endl;
            throw std::runtime_error("Shader linking failed");
        }
    }
};

class Renderer {
private:
    GLuint VAO, VBO;
    float vertices[20] = {
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
    };

public:
    Renderer() {
        setupBuffers();
    }

    ~Renderer() {
        cleanup();
    }

    void draw() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

private:
    void setupBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void cleanup() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};

class Application {
private:
    std::unique_ptr<sf::Window> window;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Renderer> renderer;
    float refractionIndex;
    bool cursorGrabbed;
    sf::Clock clock;

public:
    Application() : 
        refractionIndex(1.5f),
        cursorGrabbed(true) {
        initWindow();
        initGL();
        
        camera = std::make_unique<Camera>();
        shader = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource);
        renderer = std::make_unique<Renderer>();
        
        shader->use();
        shader->setFloat("refractionIndex", refractionIndex);
    }

    void run() {
        while (window && window->isOpen()) {
            processTime();
            processEvents();
            updateCamera();
            render();
        }
    }

private:
    void initWindow() {
        sf::ContextSettings settings;
        settings.depthBits = 24;
        settings.stencilBits = 8;
        settings.majorVersion = 4;
        settings.minorVersion = 5;
        settings.attributeFlags = sf::ContextSettings::Core;

        window = std::make_unique<sf::Window>(
            sf::VideoMode(800, 600),
            "Refraction Demo",
            sf::Style::Default,
            settings
        );

        if (window) {
            window->setVerticalSyncEnabled(true);
            window->setMouseCursorVisible(false);
            window->setMouseCursorGrabbed(true);
        }
    }

    void initGL() {
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            throw std::runtime_error("Failed to initialize GLEW");
        }

        printGLInfo();
    }

    void processTime() {
        if (camera) {
            camera->updateDeltaTime(clock.restart().asSeconds());
        }
    }

    void processEvents() {
        if (!window) return;

        sf::Event event;
        while (window->pollEvent(event)) {
            handleEvent(event);
        }
        handleInput();
    }

    void handleEvent(const sf::Event& event) {
        if (!window || !camera) return;

        if (event.type == sf::Event::Closed) {
            window->close();
        }
        else if (event.type == sf::Event::MouseMoved && cursorGrabbed) {
            camera->processMouseMovement(event.mouseMove.x, event.mouseMove.y);
        }
        else if (event.type == sf::Event::KeyPressed) {
            handleKeyPress(event.key);
        }
        else if (event.type == sf::Event::Resized) {
            glViewport(0, 0, event.size.width, event.size.height);
        }
    }

    void handleKeyPress(const sf::Event::KeyEvent& key) {
        if (!shader) return;

        if (key.code == sf::Keyboard::Tab) {
            toggleCursor();
        }
        else if (key.code == sf::Keyboard::Up) {
            adjustRefractionIndex(0.1f);
        }
        else if (key.code == sf::Keyboard::Down) {
            adjustRefractionIndex(-0.1f);
        }
        else if (key.code == sf::Keyboard::W && key.control) {
            setRefractionIndex(1.33f, "Water");
        }
        else if (key.code == sf::Keyboard::G && key.control) {
            setRefractionIndex(1.5f, "Glass");
        }
        else if (key.code == sf::Keyboard::D && key.control) {
            setRefractionIndex(2.42f, "Diamond");
        }
    }

    void handleInput() {
        if (!camera) return;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) camera->moveForward();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) camera->moveBackward();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) camera->moveLeft();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) camera->moveRight();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) camera->moveUp();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) camera->moveDown();
    }

    void updateCamera() {
        if (!shader || !camera) return;

        shader->setVec3("cameraPos", camera->getPosition());
        shader->setVec3("cameraFront", camera->getFront());
    }

    void render() {
        if (!window || !renderer) return;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        renderer->draw();
        window->display();
    }

    void toggleCursor() {
        if (!window || !camera) return;

        cursorGrabbed = !cursorGrabbed;
        window->setMouseCursorGrabbed(cursorGrabbed);
        window->setMouseCursorVisible(!cursorGrabbed);
        camera->resetFirstMouse();
    }

    void adjustRefractionIndex(float delta) {
        if (!shader) return;

        refractionIndex = std::max(1.0f, refractionIndex + delta);
        shader->setFloat("refractionIndex", refractionIndex);
        std::cout << "Refraction index: " << refractionIndex << std::endl;
    }

    void setRefractionIndex(float value, const std::string& material) {
        if (!shader) return;

        refractionIndex = value;
        shader->setFloat("refractionIndex", refractionIndex);
        std::cout << material << ": " << refractionIndex << std::endl;
    }

    void printGLInfo() {
        std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    }
};

int main() {
    try {
        Application app;
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
