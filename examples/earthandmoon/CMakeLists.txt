project(lookat)
add_executable(${PROJECT_NAME} camera.cpp ground.cpp model.cpp main.cpp openglwindow.cpp)
enable_abcg(${PROJECT_NAME})
