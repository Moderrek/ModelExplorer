#include "VBO.hpp"

VBO::VBO(const std::vector<Vertex>& vertices) {
  glGenBuffers(1, &id_);
  glBindBuffer(GL_ARRAY_BUFFER, id_);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

GLuint VBO::get_id() const {
  return id_;
}

void VBO::bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, id_);
}

void VBO::unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() const {
  glDeleteBuffers(1, &id_);
}
