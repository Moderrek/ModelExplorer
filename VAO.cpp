#include "VAO.hpp"

VAO::VAO() {
	glGenVertexArrays(1, &id_);
}

void VAO::link_attrib(const VBO& vbo, const GLuint layout, const GLuint num_components, const GLenum type, const GLsizeiptr stride, const void* offset) {
	vbo.bind();
	glVertexAttribPointer(layout, num_components, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO::unbind();
}

void VAO::bind() const {
	glBindVertexArray(id_);
}

void VAO::unbind() {
	glBindVertexArray(0);
}

void VAO::Delete() const {
	glDeleteVertexArrays(1, &id_);
}
