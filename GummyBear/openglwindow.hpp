#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include "abcg.hpp"
#include "model.hpp"

#include <random>

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  int m_viewportWidth{};
  int m_viewportHeight{};

 
  Model m_gummy;


  glm::mat4 m_modelMatrix{1.0f};
  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};

  // Shaders
  std::vector<const char*> m_shaderNames{"blinnphong"};
      
  std::vector<GLuint> m_programs;
  int m_currentProgramIndex{};

  // Light and material properties
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  
  // Gummy, Earth and stars models
  glm::vec4 m_Ka_Gummy{};
  glm::vec4 m_Kd_Gummy{};
  glm::vec4 m_Ks_Gummy{};
  float m_shininess_Gummy{};
  
  float fallGummy{3.5f};

  int X{};
  int Y{};
  int Z{};
  float B{};
  float position{};
 
  
  //degree of objects
  float degree{180.0f};
  
  std::default_random_engine m_randomEngine;
  
  void loadModel_Gummy(std::string_view path);
  void random_color(glm::vec4 *color);
  void update();
};

#endif
