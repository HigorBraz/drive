#include "ship.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Ship::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  m_rotation = 0.0f;
  m_translation = glm::vec2(0);
  m_velocity = glm::vec2(0);

  std::array<glm::vec2, 24> positions{
      // Ship body
      glm::vec2{0.0f, +10.0f},

      glm::vec2{-5.75f, +04.5f},  glm::vec2{+5.75f, +04.5f},

      glm::vec2{-5.75f, -03.5f},  glm::vec2{+5.75f, -03.5f},

      glm::vec2{-10.75f, -01.5f}, glm::vec2{+10.75f, -01.5f},
      glm::vec2{-11.75f, -05.5f}, glm::vec2{+11.75f, -05.5f},

  };

  // Normalize
  for (auto &position : positions) {
    position /= glm::vec2{15.5f, 15.5f};
  }

  const std::array indices{0, 1, 2, 1, 2, 3, 2, 3, 4, 1, 3,
                           5, 2, 4, 6, 3, 5, 7, 4, 6, 8};

  // Generate VBO
  abcg::glGenBuffers(1, &m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Ship::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform1f(m_rotationLoc, m_rotation);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 7 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Ship::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Ship::update(const GameData &gameData, float deltaTime) {
  if (gameData.m_input[static_cast<size_t>(Input::Left)] &&
      gameData.m_state == State::Playing) {
    m_translation.x -= 1 * deltaTime;
    m_rotation = 1.5707;
  }

  if (gameData.m_input[static_cast<size_t>(Input::Right)] &&
      gameData.m_state == State::Playing) {
    m_translation.x += 1 * deltaTime;
    m_rotation = 1.5 * 3.1415;
  }

  if (gameData.m_input[static_cast<size_t>(Input::Down)] &&
      gameData.m_state == State::Playing) {
    m_translation.y -= 1 * deltaTime;
    m_rotation = -3.1415;
  }

  if (gameData.m_input[static_cast<size_t>(Input::Up)] &&
      gameData.m_state == State::Playing) {
    m_translation.y += 1 * deltaTime;
    m_rotation = 0;
  }

  if (m_translation.x < -1.0f) m_translation.x += 2.0f;
  if (m_translation.x > +1.0f) m_translation.x -= 2.0f;
  if (m_translation.y < -1.0f) m_translation.y += 2.0f;
  if (m_translation.y > +1.0f) m_translation.y -= 2.0f;
}