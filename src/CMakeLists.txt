add_executable(terminal_app main.cpp)
target_link_libraries(
  terminal_app PRIVATE project_options project_warnings CONAN_PKG::docopt.cpp
                CONAN_PKG::fmt CONAN_PKG::spdlog)