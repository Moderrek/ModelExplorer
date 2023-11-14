#include "EBO.hpp"

EBO::EBO(const std::vector<GLuint>& indices) {
  glGenBuffers(1, &id_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

GLuint EBO::get_id() const {
  return id_;
}

void EBO::bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
}

void EBO::unbind() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() const {
  glDeleteBuffers(1, &id_);
}
