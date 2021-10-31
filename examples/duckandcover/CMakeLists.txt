project(duckandcover)

add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp bombs.cpp
                               duck.cpp )

enable_abcg(${PROJECT_NAME})
