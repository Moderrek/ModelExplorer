#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED

#include <string>
#include <vector>

#include "VAO.hpp"
#include "EBO.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "Vertex.hpp"

class Mesh {
  std::vector<Vertex> vertices_;
  std::vector<GLuint> indices_;
  std::vector<Texture> textures_;
  VAO vao_;

public:
  Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
  void draw(const Shader& shader, Camera& camera, glm::mat4 matrix = glm::mat4{1.0f}, glm::vec3 translation = glm::vec3{0.0f, 0.0f, 0.0f}, glm::quat rotation = glm::quat{1.0f, 0.0f, 0.0f, 0.0f}, glm::vec3 scale = glm::vec3{1.0f, 1.0f, 1.0f}) const;
};

#endif
