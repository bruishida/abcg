#include "openglwindow.hpp"

#include <imgui.h>
#include <iostream>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void OpenGLWindow::initializeGL() {
  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "depth.vert",
                                    getAssetsPath() + "depth.frag");

  // Load model
  m_model.loadObj(getAssetsPath() + "triangle.obj");
  m_model2.loadObj(getAssetsPath() + "box.obj");
  m_model3.loadObj(getAssetsPath() + "sphere.obj");
  m_model.setupVAO(m_program);
  m_model2.setupVAO(m_program);
  m_model3.setupVAO(m_program);

  // Camera at (0,0,0) and looking towards the negative z
  m_viewMatrix =
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                  glm::vec3(0.0f, 1.0f, 0.0f));
  angulo = 0.5f;
  // Setup stars
  auto &position{m_cubePositions};
  auto &rotation{m_cubeRotations};

  position = glm::vec3(0.0f, 0.0f,
                       -10.0f);

  rotation = glm::normalize(glm::vec3(1.0f,
                                      0.5f,
                                      0.0f));

  angleX = 0.3;
  angleY = 0.3;
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

  // Set uniform variables used by every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  
  float r = (-((float)rand() / RAND_MAX)) + 1;
  float g = (-((float)rand() / RAND_MAX)) + 1;
  float b = (-((float)rand() / RAND_MAX)) + 1;
    abcg::glUniform4f(colorLoc, r, g, b, 1.0f);

  // Render each object

  const auto &position{m_cubePositions};
  const auto &rotation{m_cubeRotations};
  glm::mat4 modelMatrix{1.0f};
  modelMatrix = glm::translate(modelMatrix, position);
  modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
  modelMatrix = glm::rotate(modelMatrix, m_angle, rotation);

  // Set uniform variable
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

  if (hasColided && imageRendering==1) {
    m_model2.render();
    imageRendering = 2;
  } else if(hasColided && imageRendering==2) {
    m_model3.render();
    imageRendering = 3;
  } else if(hasColided && imageRendering==3) {
    m_model.render();
    imageRendering = 1;
  } else if (imageRendering==1) {
    m_model.render();
  } else if (imageRendering==2) {
    m_model2.render();
  } else if (imageRendering==3) {
    m_model3.render();
  }

  abcg::glUseProgram(0);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {
    const auto widgetSize{ImVec2(218, 62)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportWidth - widgetSize.x - 5, 5));
    ImGui::SetNextWindowSize(widgetSize);
    ImGui::Begin("Widget window", nullptr, ImGuiWindowFlags_NoDecoration);

    {
      ImGui::PushItemWidth(120);
      static std::size_t currentIndex{};
      const std::vector<std::string> comboItems{"Perspective", "Orthographic"};

      if (ImGui::BeginCombo("Projection",
                            comboItems.at(currentIndex).c_str())) {
        for (const auto index : iter::range(comboItems.size())) {
          const bool isSelected{currentIndex == index};
          if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
            currentIndex = index;
          if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();

      ImGui::PushItemWidth(170);
      const auto aspect{static_cast<float>(m_viewportWidth) /
                        static_cast<float>(m_viewportHeight)};
      if (currentIndex == 0) {
        m_projMatrix =
            glm::perspective(glm::radians(m_FOV), aspect, 0.01f, 100.0f);

      } else {
        m_projMatrix = glm::ortho(-20.0f * aspect, 20.0f * aspect, -20.0f,
                                  20.0f, 0.01f, 100.0f);
      }
      ImGui::PopItemWidth();
    }

    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;
}

void OpenGLWindow::terminateGL() {
  m_model.terminateGL();
  m_model2.terminateGL();
  abcg::glDeleteProgram(m_program);
}

void OpenGLWindow::generateColor(){
  float r = (-((float)rand() / RAND_MAX)) + 1;
  float g = (-((float)rand() / RAND_MAX)) + 1;
  float b = (-((float)rand() / RAND_MAX)) + 1;
  
  abcg::glClearColor(r, g, b, 1);
}

void OpenGLWindow::update() {
  hasColided=false;
  
  const float deltaTime{static_cast<float>(getDeltaTime())};


  m_angle = glm::wrapAngle(m_angle + glm::radians(90.0f) * deltaTime);


  auto &position{m_cubePositions};
  auto &rotation{m_cubeRotations};

  float valorFinalX = angleX/0.5f * 0.1f;
  float valorFinalY = angleX/0.5f * 0.1f;
 

  if(position.x + valorFinalX >= 2.5f) {
    angleX = -(0.3f);
    angleY = -(0.3f);
    generateColor();
    hasColided=true;
  }

  if (position.x + valorFinalX <= -2.5f) {
    angleX = (0.1f);
    angleY = -(0.3f);
    generateColor();
    hasColided=true;
  }
  
  if(position.y + valorFinalY >= 2.5f) {
    angleY = -(0.3f);
    generateColor();
    hasColided=true;
  }

  if(position.y + valorFinalY <= -2.5f) {
    angleY = (0.3f);
    generateColor();
    hasColided=true;
  }

  valorFinalX = angleX/0.5f * 0.1f;
  valorFinalY = angleY/0.5f * 0.1f;


  position.x += valorFinalX;
  position.y += valorFinalY;
}

