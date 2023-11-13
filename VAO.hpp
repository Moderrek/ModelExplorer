#ifndef VERTEX_ARRAY_INCLUDED
#define VERTEX_ARRAY_INCLUDED

#include <glad/glad.h>
#include "VBO.hpp"

class VAO {
	GLuint id_;
public:
	VAO();
	static void link_attrib(const VBO& vbo, GLuint layout, GLuint num_components, GLenum type, GLsizeiptr stride, const void* offset);
	void bind() const;
	static void unbind();
	void Delete() const;
};

#endif