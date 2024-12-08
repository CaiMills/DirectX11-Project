#include "Camera.h"

Camera::Camera()
{
    _viewPort = { 0.0f, 0.0f, (float)_windowWidth, (float)_windowHeight, 0.0f, 1.0f };
}

Camera::~Camera()
{

}

void Camera::Update()
{
    //View
    XMStoreFloat4x4(&_view, XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_at), XMLoadFloat3(&_up)));

    //Projection
    XMMATRIX perspective = XMMatrixPerspectiveFovLH(XMConvertToRadians(90), _aspect, 0.01f, 100.0f);
    XMStoreFloat4x4(&_projection, perspective);
}