#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures) {
  vertices_ = vertices;
  indices_ = indices;
  textures_ = textures;

  vao_.bind();

  const VBO vbo{vertices};
  EBO ebo{indices};

  VAO::link_attrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), nullptr);
  VAO::link_attrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
  VAO::link_attrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
  VAO::link_attrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

  VAO::unbind();
  VBO::unbind();
  EBO::unbind();
}

void Mesh::draw(const Shader& shader, Camera& camera, glm::mat4 matrix, const glm::vec3 translation, const glm::quat rotation,
                const glm::vec3 scale) const {
  shader.activate();
  vao_.bind();
  unsigned int num_diffuse = 0;
  unsigned int num_specular = 0;

  for (unsigned int i = 0; i < textures_.size(); i++) {
    std::string num;
    std::string type = textures_[i].get_type();
    if (type == "diffuse") {
      num = std::to_string(num_diffuse++);
    } else if (type == "specular") {
      num = std::to_string(num_specular++);
    }
    textures_[i].tex_unit(shader, (type + num).c_str(), i);
    textures_[i].bind();
  }

  glUniform3f(glGetUniformLocation(shader.get_id(), "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
  camera.matrix(shader, "camMatrix");

  // Initialize matrices
  auto trans = glm::mat4(1.0f);
  auto sca = glm::mat4(1.0f);

  // Transform the matrices to their correct form
  trans = translate(trans, translation);
  auto rot = mat4_cast(rotation);
  sca = glm::scale(sca, scale);

  // Push the matrices to the vertex shader
  glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), "translation"), 1, GL_FALSE, glm::value_ptr(trans));
  glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
  glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), "scale"), 1, GL_FALSE, glm::value_ptr(sca));
  glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), "model"), 1, GL_FALSE, glm::value_ptr(matrix));

  // Draw the actual mesh
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, nullptr);
  VAO::unbind();
  Texture::unbind();
}
