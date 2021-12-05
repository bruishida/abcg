#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <tiny_obj_loader.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>
#include <glm/gtc/matrix_inverse.hpp>

#include "imfilebrowser.h"

// Explicit specialization of std::hash for Vertex
namespace std {
template <>
struct hash<Vertex> {
  size_t operator()(Vertex const& vertex) const noexcept {
    const std::size_t h1{std::hash<glm::vec3>()(vertex.position)};
    return h1;
  }
};
}  // namespace std

void OpenGLWindow::handleEvent(SDL_Event& ev) {
  if (ev.type == SDL_KEYDOWN) {
    if (ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w)
      m_dollySpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s)
      m_dollySpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_q) m_truckSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_e) m_truckSpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_k) m_planeSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_i) m_planeSpeed = 1.0f;
  }
  if (ev.type == SDL_KEYUP) {
    if ((ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_k && m_truckSpeed < 0) m_truckSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_i && m_truckSpeed > 0) m_truckSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_k && m_planeSpeed < 0) m_planeSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_i && m_planeSpeed > 0) m_planeSpeed = 0.0f;
  }
}

void OpenGLWindow::initializeGL() {
  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "shaders/texture.vert",
                                    getAssetsPath() + "shaders/texture.frag");

  // m_ground.initializeGL(m_program);

  // Load model
  loadModelFromFile(getAssetsPath() + "moon.obj");
  m_model_moon.terminateGL();

  m_model_moon.loadDiffuseTexture(getAssetsPath() + "maps/moon_diffuse.jpg");
  m_model_moon.loadNormalTexture(getAssetsPath() + "maps/moon_normal.jpg");
  m_model_moon.loadObj(getAssetsPath() + "moon.obj");
  m_model_moon.setupVAO(m_program);
  m_trianglesToDraw_moon = m_model_moon.getNumTriangles();

  m_Ka_moon = m_model_moon.getKa();
  m_Kd_moon = m_model_moon.getKd();
  m_Ks_moon = m_model_moon.getKs();
  m_shininess = 13.0f;


  loadModelFromFile(getAssetsPath() + "planet.obj");
  m_model_earth.terminateGL();

  m_model_earth.loadDiffuseTexture(getAssetsPath() + "maps/planet.jpg");
  m_model_earth.loadNormalTexture(getAssetsPath() + "maps/planet.jpg");
  m_model_earth.loadObj(getAssetsPath() + "planet.obj");
  m_model_earth.setupVAO(m_program);
  m_trianglesToDraw_earth = m_model_earth.getNumTriangles();

  m_Ka_earth = m_model_earth.getKa();
  m_Kd_earth = m_model_earth.getKd();
  m_Ks_earth = m_model_earth.getKs();



  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0]) * m_vertices.size(),
                     m_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_indices[0]) * m_indices.size(), m_indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  const GLint positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  resizeGL(getWindowSettings().width, getWindowSettings().height);
}

void OpenGLWindow::loadModelFromFile(std::string_view path) {
  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(path.data())) {
    if (!reader.Error().empty()) {
      throw abcg::Exception{abcg::Exception::Runtime(
          fmt::format("Failed to load model {} ({})", path, reader.Error()))};
    }
    throw abcg::Exception{
        abcg::Exception::Runtime(fmt::format("Failed to load model {}", path))};
  }

  if (!reader.Warning().empty()) {
    fmt::print("Warning: {}\n", reader.Warning());
  }

  const auto& attrib{reader.GetAttrib()};
  const auto& shapes{reader.GetShapes()};

  m_vertices.clear();
  m_indices.clear();

  // A key:value map with key=Vertex and value=index
  std::unordered_map<Vertex, GLuint> hash{};

  // Loop over shapes
  for (const auto& shape : shapes) {
    // Loop over indices
    for (const auto offset : iter::range(shape.mesh.indices.size())) {
      // Access to vertex
      const tinyobj::index_t index{shape.mesh.indices.at(offset)};

      // Vertex position
      const int startIndex{3 * index.vertex_index};
      const float vx{attrib.vertices.at(startIndex + 0)};
      const float vy{attrib.vertices.at(startIndex + 1)};
      const float vz{attrib.vertices.at(startIndex + 2)};

      Vertex vertex{};
      vertex.position = {vx, vy, vz};

      // If hash doesn't contain this vertex
      if (hash.count(vertex) == 0) {
        // Add this index (size of m_vertices)
        hash[vertex] = m_vertices.size();
        // Add this vertex
        m_vertices.push_back(vertex);
      }

      m_indices.push_back(hash[vertex]);
    }
  }
}

void OpenGLWindow::paintGL() {
  update();

  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  abcg::glUseProgram(m_program);

  // Get location of uniform variables (could be precomputed)
  const GLint viewMatrixLoc{
      abcg::glGetUniformLocation(m_program, "viewMatrix")};
  const GLint projMatrixLoc{
      abcg::glGetUniformLocation(m_program, "projMatrix")};
  const GLint modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};
  const GLint colorLoc{abcg::glGetUniformLocation(m_program, "color")};

  GLint normalMatrixLoc{glGetUniformLocation(m_program, "normalMatrix")};
  GLint lightDirLoc{glGetUniformLocation(m_program, "lightDirWorldSpace")};
  GLint shininessLoc{glGetUniformLocation(m_program, "shininess")};
  GLint IaLoc{glGetUniformLocation(m_program, "Ia")};
  GLint IdLoc{glGetUniformLocation(m_program, "Id")};
  GLint IsLoc{glGetUniformLocation(m_program, "Is")};
  GLint KaLoc{glGetUniformLocation(m_program, "Ka")};
  GLint KdLoc{glGetUniformLocation(m_program, "Kd")};
  GLint KsLoc{glGetUniformLocation(m_program, "Ks")};
  GLint diffuseTexLoc{glGetUniformLocation(m_program, "diffuseTex")};
  GLint normalTexLoc{glGetUniformLocation(m_program, "normalTex")};
  GLint mappingModeLoc{glGetUniformLocation(m_program, "mappingMode")};

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE,
                           &m_camera.m_projMatrix[0][0]);

  abcg::glBindVertexArray(m_VAO);

    
  glUniform1i(diffuseTexLoc, 0);
  glUniform1i(normalTexLoc, 1);
  glUniform1i(mappingModeLoc, 3);

  glUniform4fv(lightDirLoc, 1, &m_lightDir.x);
  glUniform4fv(IaLoc, 1, &m_Ia_moon.x);
  glUniform4fv(IdLoc, 1, &m_Id_moon.x);
  glUniform4fv(IsLoc, 1, &m_Is_moon.x);

  glUniform4fv(IaLoc, 1, &m_Ia_earth.x);
  glUniform4fv(IdLoc, 1, &m_Id_earth.x);
  glUniform4fv(IsLoc, 1, &m_Is_earth.x);

  float mat[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  glUniform1f(shininessLoc, 5000.0f);
  glUniform4fv(KaLoc, 1, mat);
  glUniform4fv(KdLoc, 1, mat);
  glUniform4fv(KsLoc, 1, mat);

  
  m_modelMatrix_moon = glm::mat4(1.0);
  m_modelMatrix_moon = glm::translate(m_modelMatrix_moon, glm::vec3(1.0f, 1.0f, 0.0f));
  m_modelMatrix_moon = glm::rotate(m_modelMatrix_moon, glm::radians(0.005f), glm::vec3(0, 0, 1));
  m_modelMatrix_moon = glm::scale(m_modelMatrix_moon, glm::vec3(0.2f));
  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix_moon[0][0]);

  auto modelViewMatrix{glm::mat3(m_camera.m_viewMatrix * m_modelMatrix_moon)};
  glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  m_model_moon.render(m_trianglesToDraw_moon);


  m_modelMatrix_earth = glm::mat4(1.0);
  m_modelMatrix_earth = glm::translate(m_modelMatrix_earth, glm::vec3(-0.5f, 0.0f, 0.0f));
  m_modelMatrix_earth = glm::rotate(m_modelMatrix_earth, glm::radians(0.005f), glm::vec3(0, 0, 1));
  m_modelMatrix_earth = glm::scale(m_modelMatrix_earth, glm::vec3(1.4f));
  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix_earth[0][0]);

  auto modelViewMatrix_earth{glm::mat3(m_camera.m_viewMatrix * m_modelMatrix_earth)};
  glm::mat3 normalMatrix_earth{glm::inverseTranspose(modelViewMatrix)};
  glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  m_model_earth.render(m_trianglesToDraw_earth);
  

  glUniform1f(shininessLoc, m_shininess);
  glUniform1f(shininessLoc, m_shininess);

  glUniform4fv(KaLoc, 1, &m_Ka_moon.x);
  glUniform4fv(KdLoc, 1, &m_Kd_moon.x);
  glUniform4fv(KsLoc, 1, &m_Ks_moon.x);

  glUniform4fv(KaLoc, 1, &m_Ka_earth.x);
  glUniform4fv(KdLoc, 1, &m_Kd_earth.x);
  glUniform4fv(KsLoc, 1, &m_Ks_earth.x);


  // // Draw yellow bunny
  // model = glm::mat4(1.0);
  // model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
  // model = glm::scale(model, glm::vec3(0.5f));

  // abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  // abcg::glUniform4f(colorLoc, 1.0f, 0.8f, 0.0f, 1.0f);
  // abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
  //                      nullptr);

  // // Draw blue bunny
  // model = glm::mat4(1.0);
  // model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
  // model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
  // model = glm::scale(model, glm::vec3(0.5f));

  // abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  // abcg::glUniform4f(colorLoc, 0.0f, 0.8f, 1.0f, 1.0f);
  // abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
  //                      nullptr);

  // Draw red bunny
  // model = glm::mat4(1.0);
  // model = glm::scale(model, glm::vec3(0.1f));

  // abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &model[0][0]);
  // abcg::glUniform4f(colorLoc, 1.0f, 0.25f, 0.25f, 1.0f);
  // abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
  //                      nullptr);

  abcg::glBindVertexArray(0);

  // Draw ground
  m_ground.paintGL();

  abcg::glUseProgram(0);
}

void OpenGLWindow::paintUI() { abcg::OpenGLWindow::paintUI();
{
      auto aspect{static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight)};
      m_projMatrix = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 5.0f);
    } }

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() {
  m_ground.terminateGL();

  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
  m_camera.plane(m_planeSpeed * deltaTime);
}