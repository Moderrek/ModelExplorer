#include "Texture.hpp"

Texture::Texture(const char* image, const GLenum tex_type, const GLuint slot, const GLenum format, const GLenum pixel_type) {
	type_ = tex_type;
	int width_img, height_img, num_col_ch;
	// Flips the image so its appears right side up
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &width_img, &height_img, &num_col_ch, 0);
	glGenTextures(1, &id_);
	// Assigns the texture to a Texture Unit
	glActiveTexture(GL_TEXTURE0 + slot);
	unit_ = slot;
	glBindTexture(type_, id_);

	glTexParameteri(type_, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(type_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(type_, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(type_, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(type_, 0, GL_RGBA, width_img, height_img, 0, format, pixel_type, bytes);
	glGenerateMipmap(type_);

	stbi_image_free(bytes);
	glBindTexture(type_, 0);
}

void Texture::tex_unit(const Shader& shader, const char* uniform, const GLint unit) {
	const GLint tex_uniform = glGetUniformLocation(shader.get_id(), uniform);
	shader.activate();
	glUniform1i(tex_uniform, unit);
}

GLuint Texture::get_id() const {
	return id_;
}

GLenum Texture::get_type() const {
	return type_;
}

GLuint Texture::get_unit() const {
	return unit_;
}

void Texture::bind() const {
	glActiveTexture(GL_TEXTURE0 + unit_);
	glBindTexture(type_, id_);
}

void Texture::unbind() const {
	glBindTexture(type_, 0);
}

void Texture::Delete() const {
	glDeleteTextures(1, &id_);
}
