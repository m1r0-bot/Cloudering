#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <iostream>


using namespace glm;

class Camera
{
public:
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	static Camera& getInstance() {
		static Camera instance;
		return instance;
	}

    void Init(float* width, float* height) {
        m_width = width;
        m_height = height;

        updateViewMatrix();
        initialized = true;
    }

    glm::mat4x4 getViewMatrix() const { return m_viewMatrix; }
    glm::mat4x4 getProjMatrix() const { return m_projMatrix; }
    glm::vec3 getEye() const { return m_eye; }
    glm::vec3 getUpVector() const { return m_upVector; }
    glm::vec3 getLookAt() const { return m_lookAt; }
    glm::vec3 getViewDir() const { return -glm::transpose(m_viewMatrix)[2]; }
    glm::vec3 getRightVector() const { return glm::transpose(m_viewMatrix)[0]; }
    float getFOV() const { return m_fov; }

    void setFOV(float fov)
    {
        m_fov = fov;
    }

    void updateViewMatrix()
    {
        m_viewMatrix = glm::lookAt(m_eye, m_lookAt, m_upVector);
    }

    void updateProjMatrix()
    {
        if (!initialized) return;
        m_projMatrix = glm::perspective(glm::radians(m_fov), *m_width / *m_height, 0.1f, 360.0f);
    }

    void setCameraView(glm::vec3 eye, glm::vec3 lookat)
    {
        m_eye = std::move(eye);
        m_lookAt = std::move(lookat);
        updateViewMatrix();
    }

    void processMouseDelta(glm::vec2 delta) {
        delta *= sensitivity;
        //if (isFreeCam) {
            vec3 direction = normalize(m_lookAt - m_eye);
            vec3 right = normalize(cross(direction, m_upVector));

            mat4 horizontalRotation = rotate(mat4(1.0f), radians(-delta.x), m_upVector);
            mat4 verticalRotation = rotate(mat4(1.0f), radians(delta.y), right);

            vec4 rotatedDirection = verticalRotation * horizontalRotation * vec4(direction, 1.0f);
            setCameraView(m_eye, m_eye + vec3(rotatedDirection));/*
        }
        else {
            glm::vec3 eye = m_eye - getRightVector() * delta.x;
            eye += getUpVector() * delta.y;
            eye = glm::normalize(eye) * distToTarget;
            setCameraView(eye, glm::vec3(0));
        }*/
    }

    void processMouseScroll(float y) {
        if (isFreeCam) return;
        //distToTarget -= y * scrSpeed;
        //distToTarget = glm::clamp(distToTarget, 1.0f, 25.0f);
        //setCameraView(glm::normalize(m_eye) * distToTarget, m_lookAt);
    }

    void processKeyboardInput(vec3 dir) {
        //if (!isFreeCam) return;
        dir *= camSpeed;
        vec3 offset = getViewDir() * dir.z + getRightVector() * dir.x + getUpVector() * dir.y;
        m_eye += offset;
        m_lookAt += offset;
        updateViewMatrix();
    }

    bool isFreeCam = false;
    float sensitivity = 1;
    float scrSpeed = 0.2;
    float camSpeed = 0.1;

private:
    Camera() {};
	~Camera() {}


    glm::mat4x4 m_viewMatrix;
    glm::mat4x4 m_projMatrix;
    glm::vec3 m_eye = {25, 25, 25};
    glm::vec3 m_lookAt = {0, 0, 0};
    glm::vec3 m_upVector = {0, 1, 0};
    float* m_width;
    float* m_height;
    float m_fov = 90;
    float pan_speed = .5f;

    float xAngleSub = 0.0;
    float yAngleSub = 0.0;

    float distToTarget = 5;

    bool initialized = false;
};