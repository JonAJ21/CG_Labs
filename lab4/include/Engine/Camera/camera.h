#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
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
        void updateCameraVectors()
        {
            // Calculate the new Front vector
            glm::vec3 front;
            front.x = cos(glm::radians(this->mYaw)) * cos(glm::radians(this->mPitch));
            front.y = sin(glm::radians(this->mPitch));
            front.z = sin(glm::radians(this->mYaw)) * cos(glm::radians(this->mPitch));
            this->mFront = glm::normalize(front);
            // Also re-calculate the Right and Up vector
            this->mRight = glm::normalize(glm::cross(this->mFront, this->mWorldUp)); // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
            this->mUp = glm::normalize(glm::cross(this->mRight, this->mFront));
        }

    public:
        // Constructor with vectors
        Camera(
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            GLfloat yaw = YAW,
            GLfloat pitch = PITCH)
            : mFront(glm::vec3(0.0f, 0.0f, -1.0f)),
              mMovementSpeed(SPEED),
              mMouseSensitivity(SENSITIVTY),
              mZoom(ZOOM)
        {
            this->mPosition = position;
            this->mWorldUp = up;
            this->mYaw = yaw;
            this->mPitch = pitch;
            this->updateCameraVectors();
        }
        // Constructor with scalar values
        Camera(
            GLfloat posX, GLfloat posY, GLfloat posZ,
            GLfloat upX, GLfloat upY, GLfloat upZ,
            GLfloat yaw, GLfloat pitch)
            : mFront(glm::vec3(0.0f, 0.0f, -1.0f)),
              mMovementSpeed(SPEED),
              mMouseSensitivity(SENSITIVTY),
              mZoom(ZOOM)
        {
            this->mPosition = glm::vec3(posX, posY, posZ);
            this->mWorldUp = glm::vec3(upX, upY, upZ);
            this->mYaw = yaw;
            this->mPitch = pitch;
            this->updateCameraVectors();
        }
        // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
        glm::mat4 getViewMatrix()
        {
            // return glm::lookAt(this->mPosition, this->mPosition + this->mFront, this->mUp);
            return glm::lookAt(this->mPosition, glm::vec3(0.0f, 0.0f, 0.0f), this->mUp);
        }

        glm::mat4 getProjectionMatrix()
        {
            return glm::perspective(glm::radians(this->mZoom), 800.0f / 600.0f, 0.1f, 100.0f);
        }

        // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void ProcessKeyboard(CameraMovement direction, GLfloat deltaTime)
        {
            GLfloat velocity = this->mMovementSpeed * deltaTime;
            if (direction == CameraMovement::FORWARD)
                this->mPosition += this->mFront * velocity;
            if (direction == CameraMovement::BACKWARD)
                this->mPosition -= this->mFront * velocity;
            if (direction == CameraMovement::LEFT)
                this->mPosition -= this->mRight * velocity;
            if (direction == CameraMovement::RIGHT)
                this->mPosition += this->mRight * velocity;
        }

        void ProccessMouseMovementAroundCenter(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true) {
            
            
            xoffset *= this->mMouseSensitivity;
            yoffset *= this->mMouseSensitivity;

            this->mYaw += xoffset * 0.1;
            this->mPitch += yoffset * 0.1;
            // this->mYaw += xoffset;

            // Ограничиваем углы поворота камеры
            if (constrainPitch)
            {
                if (this->mPitch > 89.0f)
                    this->mPitch = 89.0f;
                if (this->mPitch < -89.0f)
                    this->mPitch = -89.0f;
            }

            // Вычисляем новую позицию камеры   
            float radius = glm::length(this->mPosition);
            float x = radius * cos(glm::radians(this->mPitch)) * cos(glm::radians(this->mYaw));
            float y = radius * sin(glm::radians(this->mPitch));
            float z = radius * cos(glm::radians(this->mPitch)) * sin(glm::radians(this->mYaw));

            // std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
            // std::cout << "yaw: " << this->mYaw << " pitch: " << this->mPitch << std::endl;
            // std::cout << "radius: " << radius << std::endl;

            this->mPosition = glm::vec3(x, y, z);

            this->mCenter = glm::vec3(0.0f, 0.0f, 0.0f);

            this->updateCameraVectors();
        }


        void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
        {
            xoffset *= this->mMouseSensitivity;
            yoffset *= this->mMouseSensitivity;

            this->mYaw += xoffset;
            this->mPitch += yoffset;

            // Make sure that when pitch is out of bounds, screen doesn't get flipped
            if (constrainPitch)
            {
                if (this->mPitch > 89.0f)
                    this->mPitch = 89.0f;
                if (this->mPitch < -89.0f)
                    this->mPitch = -89.0f;
            }

            // Update Front, Right and Up Vectors using the updated Eular angles
            this->mCenter = this->mPosition + this->mFront;
            this->updateCameraVectors();
        }

        // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void ProcessMouseScroll(GLfloat yoffset)
        {
            if (this->mZoom >= 1.0f && this->mZoom <= 45.0f)
                this->mZoom -= yoffset;
            if (this->mZoom <= 1.0f)
                this->mZoom = 1.0f;
            if (this->mZoom >= 45.0f)
                this->mZoom = 45.0f;
        }
    };
}