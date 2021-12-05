#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <vector>

#include "abcg.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "ground.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  Camera m_camera;
  float m_dollySpeed{0.0f};
  float m_truckSpeed{0.0f};
  float m_panSpeed{0.0f};
  float m_planeSpeed{0.0f};

  Ground m_ground;

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  std::vector<Vertex> m_vertices2;
  std::vector<GLuint> m_indices2;

  glm::vec4 m_lightDir{0.5f, 0.0f, 0.0f, 0.0f};


  Model m_model;
  int m_trianglesToDraw{};
  glm::mat4 m_modelMatrix{1.0f};

  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f, 0.861f, 0.591f, 1.0f};
  //glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};

  void loadModelFromFile(std::string_view path);
  void update();
};

#endif