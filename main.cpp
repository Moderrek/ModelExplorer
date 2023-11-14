#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.hpp"


constexpr unsigned int width = 800;
constexpr unsigned int height = 800;

int main() {
  // Initialize GLFW
  glfwInit();
  // Using OpenGL 3.3
  // Tell GLFW what version of OpenGL we are using 
  // In this case we are using OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Tell GLFW we are using the CORE profile
  // So that means we only have the modern functions
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // Create GLFWwindow object of 800 by 800 pixels, naming it "Model Explorer"
  GLFWwindow* window = glfwCreateWindow(width, height, "Model Explorer", nullptr, nullptr);
  // Handle error
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window!" << std::endl;
    glfwTerminate();
    return -1;
  }
  // Introduce the window into the current context
  glfwMakeContextCurrent(window);
  // Load GLAD  so it configures OpenGL
  gladLoadGL();
  // Specify viewport of OpenGL in the Window
  glViewport(0, 0, width, height);

  Shader shader_program{"shaders/default.vert", "shaders/default.frag"};
  auto light_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  auto light_pos = glm::vec3(0.5f, 0.5f, 0.5f);
  auto light_model = glm::mat4(1.0f);
  light_model = translate(light_model, light_pos);

  shader_program.activate();
  glUniform4f(glGetUniformLocation(shader_program.get_id(), "lightColor"), light_color.x, light_color.y, light_color.z, light_color.w);
  glUniform3f(glGetUniformLocation(shader_program.get_id(), "lightPos"), light_pos.x, light_pos.y, light_pos.z);
  
  // Enables the depth buffer
  glEnable(GL_DEPTH_TEST);

  Camera camera{width, height, glm::vec3{0.0f, 0.0f, 2.0f}};
  Model model{"models/map/scene.gltf"};

  // Main while loop
  while (!glfwWindowShouldClose(window)) {
    // Background color
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    // Clean back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Handle camera inputs
    camera.inputs(window);
    // Updates and exports the camera  matrix to the Vertex Shader
    camera.update_matrix(80.0f, 0.1f, 250.0f);
    // Draw meshes
    model.draw(shader_program, camera);
    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window);
    // Take care of all GLFW events
    glfwPollEvents();
  }

  shader_program.Delete();

  // Delete window
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}