#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(const Frustrum &frustrum,
                                     const glm::vec3 &position,
                                     const glm::vec3 &lookAt,
                                     const glm::vec3 &upVector)
{
    this->CameraFrustrum = frustrum;
    this->Position = position;
    this->LookAt = lookAt;
    this->UpVector = upVector;
    RecalculateMatrix();
};

void PerspectiveCamera::RecalculateMatrix()
{
    this->ProjectionMatrix = glm::perspective(glm::radians(this->CameraFrustrum.angle),
                                              this->CameraFrustrum.width / this->CameraFrustrum.height,
                                              this->CameraFrustrum.near,
                                              this->CameraFrustrum.far);

    this->ViewMatrix = glm::lookAt(this->Position,
                                   this->LookAt,
                                   this->UpVector);

    this->ViewProjectionMatrix = this->ProjectionMatrix * this->ViewMatrix;
};