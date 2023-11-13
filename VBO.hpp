#ifndef VBO_INCLUDED
#define VBO_INCLUDED

#include <glad/glad.h>

class VBO {
	GLuint id_;
public:
	VBO(const GLfloat* vertices, GLsizeiptr size);
	inline GLuint get_id() const;
	void bind() const;
	static void unbind();
	void Delete() const;
};

#endif
