#pragma once
#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "vector2.h"
#include <string>


class InputSystem;
struct Matrix4;

enum CameraType
{
	CAMERA_FREE_CAM,
    CAMERA_FOLLOW,
};

class Camera {
public:
    Camera(int width, int height);

    void Process(float deltaTime, InputSystem& inputSystem);
    void ProcessInput(float deltaTime, InputSystem& inputSystem);

    void SetPosition(int x, int y);
    void Move(float dx, float dy);
    void Move(Vector2 dir, float deltaTime);
    void SetZoom(float zoom);
    void Zoom(float amount);
    void ZoomTowardsPoint(const Vector2& targetPoint, bool zoomIn);
    void ResetCamera();

    Vector2 GetPosition() const { return Vector2(m_fx, m_fy); }
    float GetZoom() const { return m_fzoom; }
    Matrix4 GetViewMatrix() const;
    Vector2 WorldToScreen(Vector2 worldPos) const;
    Vector2 ScreenToWorld(Vector2 screenPos) const;
    Vector2 GetViewportSize() const { return Vector2(m_fwidth, m_fheight); }
    void SetCameraType(CameraType type);
    CameraType GetCameraType() const { return m_eCameraType; }
    std::string GetCameraTypeName() const;

    void SetCamSpeed(int speed) { m_iCameraMoveSpeed = speed * 10; }
    int GetCamSpeed() const { return m_iCameraMoveSpeed; }

    bool IsEnabled() const { return m_bEnabled; }
    void Enable() { m_bEnabled = true; }
    void Disable() { m_bEnabled = false; }
protected:
    float m_fx;
    float m_fy;
    float m_fzoom;        // Zoom level
    float m_foldZoom;
    int m_fwidth;
    int m_fheight;  // Viewport dimensions
    int m_iCameraMoveSpeed;

    bool m_bEnabled;
private:
    CameraType m_eCameraType;
    //InputSystem& m_inputSystem;
};
#endif // __CAMERA_H_