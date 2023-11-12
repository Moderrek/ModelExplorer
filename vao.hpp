#ifndef VERTEX_ARRAY_INCLUDED
#define VERTEX_ARRAY_INCLUDED

#include <glad/glad.h>
#include "vbo.hpp"

class VAO {
public:
	GLuint ID;
	VAO();
	void LinkVBO(VBO vbo, GLuint layout);
	void Bind();
	void Unbind();
	void Delete();
};

#endif