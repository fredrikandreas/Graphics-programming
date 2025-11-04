# Create GeometricTools library
add_library(GeometricTools INTERFACE)

# Create alias so it can be used as Framework::GeometricTools
add_library(Framework::GeometricTools ALIAS GeometricTools)

# Provide include path to others
target_include_directories(GeometricTools INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Link dependencies like GLAD
target_link_libraries(GeometricTools INTERFACE glad)