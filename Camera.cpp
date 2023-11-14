#include "Camera.hpp"

Camera::Camera(const int width, const int height, const glm::vec3 position) {
  Camera::width = width;
  Camera::height = height;
  Position = position;
}

void Camera::update_matrix(const float fov_deg, const float near_plane, const float far_plane) {
  const auto view = lookAt(Position, Position + Orientation, Up);
  const float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
  const auto projection = glm::perspective(glm::radians(fov_deg), aspect_ratio, near_plane, far_plane);
  cameraMatrix = projection * view;
}

void Camera::matrix(const Shader& shader, const char* uniform) {
  glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), uniform), 1, GL_FALSE, value_ptr(cameraMatrix));
}

void Camera::inputs(GLFWwindow* window, const float delta_time) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    Position += speed * delta_time * Orientation;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    Position += speed * delta_time * -Orientation;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    Position += speed * delta_time * -normalize(cross(Orientation, Up));
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    Position += speed * delta_time * normalize(cross(Orientation, Up));
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    Position += speed * delta_time * Up;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
    Position += speed * delta_time * -Up;
  }
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    const auto center = glm::vec2{static_cast<double>(width) / 2, static_cast<double>(height) / 2};
    if (firstClick) {
      firstClick = false;
      // Center mouse pos
      glfwSetCursorPos(window, center.x, center.y);
    }
    double mouse_x, mouse_y;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    const float rot_x = sensitivity * static_cast<float>(mouse_y - center.x) / static_cast<float>(width);
    const float rot_y = sensitivity * static_cast<float>(mouse_x - center.y) / static_cast<float>(height);

    const glm::vec3 new_orientation = rotate(Orientation, glm::radians(-rot_x), normalize(cross(Orientation, Up)));
    if (!(angle(new_orientation, Up) <= glm::radians(5.0f)
      || angle(new_orientation, -Up) <= glm::radians(5.0f))) {
      Orientation = new_orientation;
    }

    Orientation = rotate(Orientation, glm::radians(-rot_y), Up);

    glfwSetCursorPos(window, center.x, center.y);
  }
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    firstClick = true;
  }
}
