#include "include/GameCamera.h"

GameCamera::GameCamera(const GameCameraConfig &cfg)
    : m_cfg(cfg),
      m_persp({cfg.fov, cfg.width, cfg.height, cfg.nearPlane, cfg.farPlane},
              cfg.eye, cfg.at, cfg.up),
      m_ortho({-2.0f, 2.0f, -2.0f, 2.0f, 1.0f, 10.0f},
              {0.0f, 1.0f, 5.0f}, 0.0f)
{
}

PerspectiveCamera &GameCamera::perspective() { return m_persp; }
OrthographicCamera &GameCamera::orthographic() { return m_ortho; }
