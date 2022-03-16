project(Game)
add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp balls.cpp
                               ship.cpp)
enable_abcg(${PROJECT_NAME})
