#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(const Frustrum &frustrum,
                                       const glm::vec3 &position,
                                       float rotation)
{
    this->CameraFrustrum = frustrum;
    this->Position = position;
    this->Rotation = rotation;
    RecalculateMatrix();
};

void OrthographicCamera::RecalculateMatrix()
{
    this->ProjectionMatrix = glm::ortho(this->CameraFrustrum.left,
                                        this->CameraFrustrum.right,
                                        this->CameraFrustrum.bottom,
                                        this->CameraFrustrum.top,
                                        this->CameraFrustrum.near,
                                        this->CameraFrustrum.far);

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), this->Position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(this->Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

    this->ViewMatrix = glm::inverse(transform);

    this->ViewProjectionMatrix = this->ProjectionMatrix * this->ViewMatrix;
};