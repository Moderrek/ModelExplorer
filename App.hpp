#ifndef APP_HPP_INCLUDED
#define APP_HPP_INCLUDED

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "App.hpp"
#include "Model.hpp"

class App {
  GLFWwindow* window_;
  Shader* shader_program_;
  Shader* shader_outlining_program_;
  Shader* shader_grass_program_;
  
  int window_width_;
  int window_height_;
  const char* window_title_;
public:
  App(int window_width, int window_height, const char* title);
  ~App();
  void init();
  void run() const;
};

#endif