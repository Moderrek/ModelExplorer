#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "Shader.hpp"

class Texture {
	GLuint id_;
	GLenum type_;
	GLuint unit_;
public:
	Texture(const char* image, GLenum tex_type, GLuint slot, GLenum format, GLenum pixel_type);
	static void tex_unit(const Shader& shader, const char* uniform, GLint unit);
	GLuint get_id() const;
	GLenum get_type() const;
	GLuint get_unit() const;
	void bind() const;
	void unbind() const;
	void Delete() const;
};

#endif