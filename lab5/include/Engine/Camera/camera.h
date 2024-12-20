#pragma once

#include "GL/glew.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.015f;
const GLfloat ZOOM = 45.0f;


namespace Engine
{
    class Camera
    {
    private:
        // Camera Attributes
        glm::vec3 mPosition;
        glm::vec3 mFront;
        glm::vec3 mUp;
        glm::vec3 mRight;
        glm::vec3 mWorldUp;
        // Eular Angles
        GLfloat mYaw;
        GLfloat mPitch;
        // Camera options
        GLfloat mMovementSpeed;
        GLfloat mMouseSensitivity;
        GLfloat mZoom;

        glm::vec3 mCenter;

        // Calculates the front vector from the Camera's (updated) Eular Angles
        void updateCameraVectors();

    public:
        // Constructor with vectors
        Camera(
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            GLfloat yaw = YAW,
            GLfloat pitch = PITCH
        );
        // Constructor with scalar values
        Camera(
            GLfloat posX, GLfloat posY, GLfloat posZ,
            GLfloat upX, GLfloat upY, GLfloat upZ,
            GLfloat yaw, GLfloat pitch
        );
        
        // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
        glm::mat4 getViewMatrix();

        glm::mat4 getProjectionMatrix();

        // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void ProcessKeyboard(CameraMovement direction, GLfloat deltaTime);

        void ProccessMouseMovementAroundCenter(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
        
        void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

        // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void ProcessMouseScroll(GLfloat yoffset);
    };
}