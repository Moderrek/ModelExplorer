#ifndef EBO_INCLUDED
#define EBO_INCLUDED

#include <glad/glad.h>

class EBO {
	GLuint id_;
public:
	EBO(const GLuint* indices, GLsizeiptr size);
	GLuint get_id() const;
	void bind() const;
	static void unbind();
	void Delete() const;
};

#endif
