#ifndef VERTEX_ARRAY_INCLUDED
#define VERTEX_ARRAY_INCLUDED

#include <glad/glad.h>
#include "vbo.hpp"

class VAO {
public:
	GLuint ID;
	VAO();
	void LinkAttrib(VBO vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();
};

#endif