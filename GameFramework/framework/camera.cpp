#include "camera.h"
#include "matrix4.h"
#include "logmanager.h"
#include "inputsystem.h"

#include <utility>
#include <iostream>
#include <assert.h>
#include <utility>

Camera::Camera(int width, int height)
    : m_fx(0)
    , m_fy(0)
    , m_foldZoom(1.0f)
    , m_fzoom(1.0f)
    , m_fwidth(width)
    , m_fheight(height)
    , m_eCameraType(CAMERA_FREE_CAM) // Default to free camera
    , m_iCameraMoveSpeed(300) // Default camera move speed
    , m_bEnabled(false)
    //, m_inputSystem(inputSystem)
{
    if(this == nullptr) {
		LogManager::GetInstance().Log("[Info] Camera initialization failed!");
		assert(0);
	}
    LogManager::GetInstance().Log("[Info] Camera initialized successfully!");
}

void Camera::Process(float deltaTime, InputSystem& inputSystem)
{
    if(!m_bEnabled) return; // Only process if camera is enabled
    switch(m_eCameraType) {
		case CAMERA_FREE_CAM:
			ProcessInput(deltaTime, inputSystem);
			break;
		case CAMERA_FOLLOW:
			// Implement follow camera logic here
			break;
		default:
			LogManager::GetInstance().Log("[Error] Unknown camera type!");
			break;
	}
}

void Camera::ProcessInput(float deltaTime, InputSystem& inputSystem)
{
    Vector2 dir = { 0, 0 };
    if (inputSystem.GetKeyState(SDL_SCANCODE_W)) dir.y = -1;
    if (inputSystem.GetKeyState(SDL_SCANCODE_S)) dir.y = 1;
    if (inputSystem.GetKeyState(SDL_SCANCODE_A)) dir.x = -1;
    if (inputSystem.GetKeyState(SDL_SCANCODE_D)) dir.x = 1;
    Move(dir, deltaTime);
    
    /*moveVector = { dir.x, dir.y };*/
}

void Camera::SetPosition(int x, int y) {
    m_fx = x;
    m_fy = y;

}

void Camera::Move(float dx, float dy) {

    Vector2 toMove = {dx, dy};
   /* toMove.Normalize();*/

    m_fx += toMove.x;
    m_fy += toMove.y;
}

void Camera::Move(Vector2 dir, float deltaTime)
{
    if(dir.x != 0 && dir.y != 0) // Check if the direction is not zero
		dir.Normalize(); // Normalize the direction vector to get a unit vector
    //std::cout << "Vector2 dir: " << dir.x << ", " << dir.y << std::endl;
    m_fx += dir.x * m_iCameraMoveSpeed * deltaTime;
    m_fy += dir.y * m_iCameraMoveSpeed * deltaTime;
}

void Camera::SetZoom(float zoom) {
    m_fzoom = std::max(zoom, 0.1f); // Prevent division by zero
}

void Camera::Zoom(float amount) {
    m_fzoom *= amount;
    m_fzoom = std::max(m_fzoom, 0.1f);
}

static constexpr float ZOOM_SENS = 0.1f;

void Camera::ZoomTowardsPoint(const Vector2& targetPoint, bool zoomIn) {
    //std::cout << "Before zoom: Position=(" << m_fx << "," << m_fy << "), Zoom=" << m_fzoom << std::endl;
    // Store current state
    float oldZoom = m_fzoom;
    Vector2 oldPos(m_fx, m_fy);

    // Apply zoom (smaller increment for smoother control)
    static constexpr float ZOOM_SENS = 0.05f;
    m_fzoom += zoomIn ? ZOOM_SENS : -ZOOM_SENS;
    m_fzoom = std::max(m_fzoom, 0.1f);

    // Simple proportional movement
    float zoomFactor = m_fzoom / oldZoom;
    Vector2 delta = targetPoint - oldPos;

    // Move camera proportionally to zoom change
    m_fx -= delta.x * (1.0f - zoomFactor);
    m_fy -= delta.y * (1.0f - zoomFactor);

    //std::cout << "After zoom: Position=(" << m_fx << "," << m_fy << "), Zoom=" << m_fzoom << std::endl;
}

void Camera::ResetCamera()
{
	m_fx = 0.0f;
	m_fy = 0.0f;
	m_fzoom = 1.0f;
}

Matrix4 Camera::GetViewMatrix() const {
    Matrix4 view;
    SetIdentity(view);
    /*for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            std::cout << "[" << i << "][" << j << "] " << view.m[i][j] << std::endl;*/
    // Translate to negative camera position (inverse operation)
    view.m[3][0] = -m_fx;
    view.m[3][1] = -m_fy;

    // Apply zoom
    view.m[0][0] = m_fzoom;
    view.m[1][1] = m_fzoom;

    return view;
}

Vector2 Camera::WorldToScreen(Vector2 worldPos) const {
    Vector2 screenPos;
    screenPos.x = (worldPos.x - m_fx) * m_fzoom + m_fwidth / 2.0f;
    screenPos.y = -(worldPos.y - m_fy) * m_fzoom + m_fheight / 2.0f;
    return screenPos;
}



Vector2 Camera::ScreenToWorld(Vector2 screenPos) const {
    Vector2 worldPos;
    worldPos.x = (((screenPos.x - m_fwidth / 2.0f) / m_fzoom) + m_fx);
    worldPos.y = -(((screenPos.y - m_fheight / 2.0f) / m_fzoom) + m_fy);
    return worldPos;
}

void Camera::SetCameraType(CameraType type) {
    m_eCameraType = type;
    //std::string logMessage = ("[Camera] Type changed to: ") + (type == CAMERA_FREE_CAM) ? "Free Cam" : "Follow";
    std::string logMessage = "[Camera] Type changed to: " + GetCameraTypeName();
    LogManager::GetInstance().Log(logMessage.c_str());
}

std::string Camera::GetCameraTypeName() const {
	switch (m_eCameraType) {
		case CAMERA_FREE_CAM:
			return "Free Cam";
		case CAMERA_FOLLOW:
			return "Follow";
		default:
			return "Unknown";
	}
}