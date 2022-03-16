#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "imfilebrowser.h"

void OpenGLWindow::initializeGL() {
  X = 1;
  Y = 1;
  Z = 1;
  B = 1.0f;

  std::uniform_real_distribution<float> pd(-2.0f, 2.0f);
  position = pd(m_randomEngine);

  abcg::glClearColor(0.9f, 0.9f, 0.9f, 1);
  abcg::glEnable(GL_DEPTH_TEST);

  const auto program{
      createProgramFromFile(getAssetsPath() + "shaders/blinnphong.vert",
                            getAssetsPath() + "shaders/blinnphong.frag")};

  m_programs.push_back(program);

  // Load default models
  loadModel_Gummy(getAssetsPath() + "gummy.obj");
}

void OpenGLWindow::random_color(glm::vec4 *color) {
  std::uniform_real_distribution<float> rd(0.0f, 1.0f);
  std::uniform_real_distribution<float> gd(0.0f, 1.0f);
  std::uniform_real_distribution<float> bd(0.0f, 1.0f);

  *color = glm::vec4(rd(m_randomEngine), gd(m_randomEngine), bd(m_randomEngine),
                     1.0f);
}

// function for load gummy model
void OpenGLWindow::loadModel_Gummy(std::string_view path) {
  m_gummy.terminateGL();

  m_gummy.loadObj(path);
  m_gummy.setupVAO(m_programs.at(m_currentProgramIndex));

  random_color(&m_Ka_Gummy);
  random_color(&m_Kd_Gummy);
  random_color(&m_Ks_Gummy);
  m_shininess_Gummy = m_gummy.getShininess();
}

void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  // Use currently selected program
  const auto program{m_programs.at(m_currentProgramIndex)};
  abcg::glUseProgram(program);

  // Get location of uniform variables
  const GLint viewMatrixLoc{abcg::glGetUniformLocation(program, "viewMatrix")};
  const GLint projMatrixLoc{abcg::glGetUniformLocation(program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(program, "modelMatrix")};
  const GLint normalMatrixLoc{
      abcg::glGetUniformLocation(program, "normalMatrix")};
  const GLint lightDirLoc{
      abcg::glGetUniformLocation(program, "lightDirWorldSpace")};
  const GLint shininessLoc{abcg::glGetUniformLocation(program, "shininess")};
  const GLint IaLoc{abcg::glGetUniformLocation(program, "Ia")};
  const GLint IdLoc{abcg::glGetUniformLocation(program, "Id")};
  const GLint IsLoc{abcg::glGetUniformLocation(program, "Is")};
  const GLint KaLoc{abcg::glGetUniformLocation(program, "Ka")};
  const GLint KdLoc{abcg::glGetUniformLocation(program, "Kd")};
  const GLint KsLoc{abcg::glGetUniformLocation(program, "Ks")};
  const GLint diffuseTexLoc{abcg::glGetUniformLocation(program, "diffuseTex")};
  const GLint normalTexLoc{abcg::glGetUniformLocation(program, "normalTex")};
  const GLint mappingModeLoc{
      abcg::glGetUniformLocation(program, "mappingMode")};

  // Set uniform variables used by every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(normalTexLoc, 1);
  abcg::glUniform1i(mappingModeLoc, 0);

  const auto lightDirRotated{m_lightDir};
  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);

  const auto modelViewMatrix{glm::mat3(m_viewMatrix * m_modelMatrix)};
  glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  glm::mat4 modelMatrix{1.0f};

  // render gummy
  abcg::glUniform1f(shininessLoc, m_shininess_Gummy);
  abcg::glUniform4fv(KaLoc, 1, &m_Ka_Gummy.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd_Gummy.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks_Gummy.x);

  modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));

  modelMatrix =
      glm::translate(modelMatrix, glm::vec3(position, fallGummy, -10.0f));
  modelMatrix =
      glm::rotate(modelMatrix, glm::radians(degree), glm::vec3(X, 1, Z));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

  m_gummy.render(m_gummy.getNumTriangles());
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();
  const auto aspect{static_cast<float>(m_viewportWidth) /
                    static_cast<float>(m_viewportHeight)};
  m_projMatrix = glm::perspective(glm::radians(30.0f), aspect, 0.01f, 100.0f);
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;
}

void OpenGLWindow::terminateGL() { m_gummy.terminateGL(); }

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  degree += 0.5f * B;
  fallGummy += -2.0f * deltaTime;

  // turn back gummy bear for up the camera
  if (fallGummy < -3.5) {
    fallGummy = 3.5;
    random_color(&m_Ka_Gummy);
    random_color(&m_Kd_Gummy);
    random_color(&m_Ks_Gummy);

    std::uniform_real_distribution<float> xd(0.0f, 10.0f);
    std::uniform_real_distribution<float> yd(0.0f, 10.0f);
    std::uniform_real_distribution<float> zd(0.0f, 10.0f);
    std::uniform_real_distribution<float> bd(0.0f, 10.0f);
    std::uniform_real_distribution<float> pd(-2.0f, 2.0f);

    position = pd(m_randomEngine);

    if (bd(m_randomEngine) > 5.0f) {
      B = 1.0f;
    } else {
      B = -1.0f;
    }

    X = 0;
    Y = 0;
    Z = 0;

    while (X == 0 && Y == 0 && Z == 0) {
      if (xd(m_randomEngine) > 5.5f) {
        X = 1;
      } else {
        X = 0;
      }

      if (yd(m_randomEngine) > 5.5f) {
        Y = 1;
      } else {
        Y = 0;
      }

      if (zd(m_randomEngine) > 5.5f) {
        Z = 1;
      } else {
        Z = 0;
      }
    }
  }
}