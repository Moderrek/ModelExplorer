#ifndef EBO_INCLUDED
#define EBO_INCLUDED

#include <glad/glad.h>

class EBO {
public:
	GLuint ID;
	EBO(GLuint* indices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif
