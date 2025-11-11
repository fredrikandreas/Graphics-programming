#include <glm/glm.hpp>
#include <PerspectiveCamera.h>
#include <OrthographicCamera.h>

struct GameCameraConfig
{
    float fov = 45.0f;
    float width = 800.0f;
    float height = 600.0f;
    float nearPlane = 0.25f;
    float farPlane = 20.0f;
    glm::vec3 eye = {0.0f, 4.0f, 7.0f};
    glm::vec3 at = {0.0f, 0.0f, 0.0f};
    glm::vec3 up = {0.0f, 1.0f, 0.0f};
};

class GameCamera
{
public:
    explicit GameCamera(const GameCameraConfig &cfg = {});
    PerspectiveCamera &perspective();
    OrthographicCamera &orthographic();
    const glm::vec3 &eye() const { return m_cfg.eye; }

private:
    GameCameraConfig m_cfg;
    PerspectiveCamera m_persp;
    OrthographicCamera m_ortho;
};
