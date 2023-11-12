#ifndef VBO_INCLUDED
#define VBO_INCLUDED

#include <glad/glad.h>

class VBO {
public:
	GLuint ID;
	VBO(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif
