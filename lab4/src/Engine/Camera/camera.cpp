#include "Engine/Camera/camera.h"

namespace Engine {

    void Camera::updateCameraVectors() {
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

    Camera::Camera(
        glm::vec3 position,
        glm::vec3 up,
        GLfloat yaw,
        GLfloat pitch)
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

    Camera::Camera(
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

    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(this->mPosition, this->mPosition + this->mFront, this->mUp);
    }

    glm::mat4 Camera::getProjectionMatrix() {
        return glm::perspective(glm::radians(this->mZoom), 800.0f / 600.0f, 0.1f, 100.0f);
    }

    void Camera::ProcessKeyboard(CameraMovement direction, GLfloat deltaTime) {
        GLfloat velocity = this->mMovementSpeed * deltaTime;
        if (direction == CameraMovement::FORWARD)
            this->mPosition += this->mFront * velocity;
        if (direction == CameraMovement::BACKWARD)
            this->mPosition -= this->mFront * velocity;
        if (direction == CameraMovement::LEFT)
            this->mPosition -= this->mRight * velocity;
        if (direction == CameraMovement::RIGHT)
            this->mPosition += this->mRight * velocity; 
        if (direction == CameraMovement::UP)
            this->mPosition += this->mUp * velocity;
        if (direction == CameraMovement::DOWN)
            this->mPosition -= this->mUp * velocity;
    }

    void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch) {
        xoffset *= this->mMouseSensitivity;
        yoffset *= this->mMouseSensitivity;

        this->mYaw += xoffset;
        this->mPitch += yoffset;

        if (constrainPitch) {
            if (this->mPitch > 89.0f)
                this->mPitch = 89.0f;
            if (this->mPitch < -89.0f)
                this->mPitch = -89.0f;
        }
        this->mCenter = this->mPosition + this->mFront;
        this->updateCameraVectors();
    }

    void Camera::ProcessMouseScroll(GLfloat yoffset) {
        if (this->mZoom >= 1.0f && this->mZoom <= 45.0f)
            this->mZoom -= yoffset;
        if (this->mZoom <= 1.0f)
            this->mZoom = 1.0f;
        if (this->mZoom >= 45.0f)
            this->mZoom = 45.0f;
    }

    void Camera::ProccessMouseMovementAroundCenter(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch) {
        xoffset *= this->mMouseSensitivity;
        yoffset *= this->mMouseSensitivity;

        this->mYaw += xoffset * 0.1;
        this->mPitch += yoffset * 0.1;

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

        this->mPosition = glm::vec3(x, y, z);

        this->mCenter = glm::vec3(0.0f, 0.0f, 0.0f);

        this->updateCameraVectors();
    }
}   