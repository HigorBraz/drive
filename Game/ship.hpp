#ifndef SHIP_HPP_
#define SHIP_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

class Balls;
class OpenGLWindow;

class Ship {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(const GameData &gameData, float deltaTime);
  void setRotation(float rotation) { m_rotation = rotation; }

 private:
  friend Balls;
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  glm::vec4 m_color{0.677, 0.812, 0.137, 1};
  float m_rotation{};
  float m_scale{0.2f};
  glm::vec2 m_translation{glm::vec2(0)};
  glm::vec2 m_velocity{glm::vec2(0)};
  
  float count = 3.0f;
};
#endif
