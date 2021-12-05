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


  Model m_model_moon;
  int m_trianglesToDraw_moon{};
  glm::mat4 m_modelMatrix_moon{1.0f};

  glm::mat4 m_viewMatrix_moon{1.0f};
  glm::mat4 m_projMatrix{1.0f};
  glm::vec4 m_Ia_moon{1.0f};
  glm::vec4 m_Id_moon{1.0f};
  glm::vec4 m_Is_moon{1.0f, 0.861f, 0.591f, 1.0f};
  //glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka_moon{};
  glm::vec4 m_Kd_moon{};
  glm::vec4 m_Ks_moon{};
  float m_shininess{};


  Model m_model_earth;
  int m_trianglesToDraw_earth{};
  glm::mat4 m_modelMatrix_earth{1.0f};

  glm::mat4 m_viewMatrix_earth{1.0f};
  // glm::mat4 m_projMatrix_earth{1.0f};
  glm::vec4 m_Ia_earth{1.0f};
  glm::vec4 m_Id_earth{1.0f};
  glm::vec4 m_Is_earth{1.0f, 0.861f, 0.591f, 1.0f};
  //glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka_earth{};
  glm::vec4 m_Kd_earth{};
  glm::vec4 m_Ks_earth{};
  //float m_shininess_earth{};

  void loadModelFromFile(std::string_view path);
  void update();
};

#endif