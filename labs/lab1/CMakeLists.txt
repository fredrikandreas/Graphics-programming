add_executable(lab1 main.cpp)

target_include_directories(lab1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lab1 PRIVATE glfw glad)
