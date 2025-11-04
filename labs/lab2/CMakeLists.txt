add_executable(lab2 main.cpp)

target_include_directories(lab2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lab2 PRIVATE glfw glad)
