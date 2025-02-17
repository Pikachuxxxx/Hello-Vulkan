#include "Camera3D.h"

// Constructor with vectors
Camera3D::Camera3D(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
    this->Position = position;
    this->WorldUp = up;
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
}

// Constructor with scalar values
Camera3D::Camera3D(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
    this->Position = glm::vec3(posX, posY, posZ);
    this->WorldUp = glm::vec3(upX, upY, upZ);
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
}

void Camera3D::Update(Window& window, float deltaTime)
{
    if (window.isKeyHeld(GLFW_KEY_W) || window.isKeyHeld(GLFW_KEY_UP))
        ProcessKeyboard(FORWARD, deltaTime);
    else if (window.isKeyHeld(GLFW_KEY_S) || window.isKeyHeld(GLFW_KEY_DOWN))
        ProcessKeyboard(BACKWARD, deltaTime);
    if (window.isKeyHeld(GLFW_KEY_D) || window.isKeyHeld(GLFW_KEY_RIGHT))
        ProcessKeyboard(RIGHT, deltaTime);
    else if (window.isKeyHeld(GLFW_KEY_A) || window.isKeyHeld(GLFW_KEY_LEFT))
        ProcessKeyboard(LEFT, deltaTime);

    if (window.isMouseButtonHeld(GLFW_MOUSE_BUTTON_RIGHT))
    {
        ProcessMouseMovement(window.deltaMouseX, window.deltaMouseY);
        window.deltaMouseX = 0.0f;
        window.deltaMouseY = 0.0f;
    }
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera3D::GetViewMatrix()
{
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

glm::mat4 Camera3D::GetViewMatrixLH()
{
    return glm::lookAtLH(this->Position, this->Position + this->Front, this->Up);
}

glm::mat4 Camera3D::GetViewMatrixRH()
{
    return glm::lookAtRH(this->Position, this->Position + this->Front, this->Up);
}
// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera3D::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = this->MovementSpeed;// * deltaTime;

    if (direction == FORWARD)
        this->Position += this->Front * velocity;
    if (direction == BACKWARD)
        this->Position -= this->Front * velocity;
    if (direction == LEFT)
        this->Position -= this->Right * velocity;
    if (direction == RIGHT)
        this->Position += this->Right * velocity;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera3D::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    this->Yaw   += xoffset;
    this->Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    this->updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera3D::ProcessMouseScroll(float yoffset)
{
    if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
        this->Zoom -= yoffset;
    if (this->Zoom <= 1.0f)
        this->Zoom = 1.0f;
    if (this->Zoom >= 45.0f)
        this->Zoom = 45.0f;
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera3D::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    front.y = sin(glm::radians(this->Pitch));
    front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    this->Front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->Up    = glm::normalize(glm::cross(this->Right, this->Front));
}
