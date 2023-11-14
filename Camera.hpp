#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Shader.hpp"

class Camera {
public:
  glm::vec3 Position;
  glm::vec3 Orientation = glm::vec3{0.0f, 0.0f, -1.0f};
  glm::vec3 Up = glm::vec3{0.0f, 1.0f, 0.0f};
  glm::mat4 cameraMatrix = glm::mat4{1.0f};

  bool firstClick = true;

  int width;
  int height;

  float speed = 0.1f;
  float sensitivity = 100.0f;

  Camera(int width, int height, glm::vec3 position);

  void update_matrix(float fov_deg, float near_plane, float far_plane);
  void matrix(const Shader& shader, const char* uniform);
  void inputs(GLFWwindow* window);
};

#endif
