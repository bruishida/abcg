#include "openglwindow.hpp"

#include <imgui.h>

#include "abcg.hpp"

void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.set(static_cast<size_t>(Input::JumpUp));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.set(static_cast<size_t>(Input::JumpDown));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(static_cast<size_t>(Input::JumpUp));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.reset(static_cast<size_t>(Input::JumpDown));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
  }

}

void OpenGLWindow::initializeGL() {
  // Load a new font
  ImGuiIO &io{ImGui::GetIO()};
  const auto filename{getAssetsPath() + "Inconsolata-Medium.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }
  
  // Create program to render the other objects
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                           getAssetsPath() + "objects.frag");

  abcg::glClearColor(0.3, 0.5, 1, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  restart();
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;
  m_restartWaitTimer.restart();
  m_duck.initializeGL(m_objectsProgram);
  m_asteroids.initializeGL(m_objectsProgram, 3);
}

void OpenGLWindow::update() {
  const float deltaTime{static_cast<float>(getDeltaTime())};

  // Wait 5 seconds before restarting

  if (m_gameData.m_state == State::Go && m_restartWaitTimer.elapsed() > 2) {
    restart();
    return;
  }

  if (m_gameData.m_state == State::Ready && m_restartWaitTimer.elapsed() > 2) {
    m_gameData.m_state = State::Go;
    m_restartWaitTimer.restart();
  }

  if (m_gameData.m_state == State::GameOver || m_gameData.m_state == State::Win) {
    if (m_restartWaitTimer.elapsed() > 4) {
      m_gameData.m_state = State::Ready;
      m_restartWaitTimer.restart();
    }
  }
  
  m_duck.update(m_gameData, deltaTime);
  m_asteroids.update(m_duck, deltaTime);

  if (m_gameData.m_state == State::Playing) {
    checkCollisions();
    checkWinCondition();
  }
}

void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  m_asteroids.paintGL();
  m_duck.paintGL(m_gameData);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {
    const auto size{ImVec2(300, 85)};
    const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                               (m_viewportHeight - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    if (m_gameData.m_state == State::GameOver) {
      ImGui::Text("Game Over!");
    } else if (m_gameData.m_state == State::Win) {
      ImGui::Text("*You Win!*");
    } else if (m_gameData.m_state == State::Ready) {
      ImGui::Text("*Ready?*");
    } else if (m_gameData.m_state == State::Go) {
      ImGui::Text("*Go!*");
    }

    ImGui::PopFont();
    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
    abcg::glDeleteProgram(m_objectsProgram);

  m_asteroids.terminateGL();
  m_duck.terminateGL();
}

void OpenGLWindow::checkCollisions() {
  // Check collision between duck and asteroids
  for (const auto &asteroid : m_asteroids.m_asteroids) {
    const auto asteroidTranslation{asteroid.m_translation};
    const auto distance{
        glm::distance(m_duck.m_translation, asteroidTranslation)};

    if (distance < m_duck.m_scale * 0.6f + asteroid.m_scale * 0.5f) {
      m_gameData.m_state = State::GameOver;
      m_restartWaitTimer.restart();
    }
  }
}

void OpenGLWindow::checkWinCondition() {
  if (m_restartWaitTimer.elapsed() > 10) {
    m_gameData.m_state = State::Win;
    m_restartWaitTimer.restart();
  }
}