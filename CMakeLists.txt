cmake_minimum_required(VERSION 2.6)
project(Osmos-Lite CXX)

# We follow the Semver convention
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

# TO-DO Add custom extension for executable types (e.g. make them end in
# `.out') so as to simplify `.gitignore' rules.

# configure_file(
#     "ProjectConfig.hpp.in" "${PROJECT_BINARY_DIR}/ProjectConfig.hpp"
# )

add_library(Observer Observer.cpp)

add_library(Parsys Parsys.cpp)
target_link_libraries(Parsys Observer)

add_library(ParsysView ParsysView.cpp)
target_link_libraries(ParsysView Parsys Observer)

add_library(EventManager EventManager.cpp)
target_link_libraries(EventManager Parsys ParsysView Observer)

add_executable(osmos main.cpp)
target_link_libraries(
	osmos
	Parsys ParsysView Observer EventManager
    sfml-graphics sfml-window sfml-system
)

add_subdirectory(test)
