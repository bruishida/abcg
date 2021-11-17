#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <random>

#include "abcg.hpp"
#include "model.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  static const int m_numStars{1};

  GLuint m_program{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  std::default_random_engine m_randomEngine;

  Model m_model;
  Model m_model2;
  Model m_model3;

  glm::vec3 m_cubePositions;
  glm::vec3 m_cubeRotations;
  float angleX{};
  float angleY{};
  float m_angle{};

  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};
  float angulo;
  float m_FOV{30.0f};
  void generateColor();
  void update();
  bool hasColided = false;
  int imageRendering = 1;
};

#endif