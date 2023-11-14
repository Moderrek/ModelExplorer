#include "Texture.hpp"

#include <stb/stb_image.h>

Texture::Texture(const char* image, const char* tex_type, const GLuint slot) {
  type_ = tex_type;
  int width_img, height_img, color_channel_count;
  // Flips the image so its appears right side up
  stbi_set_flip_vertically_on_load(true);
  unsigned char* bytes = stbi_load(image, &width_img, &height_img, &color_channel_count, 0);
  glGenTextures(1, &id_);
  // Assigns the texture to a Texture Unit
  glActiveTexture(GL_TEXTURE0 + slot);
  unit_ = slot;
  glBindTexture(GL_TEXTURE_2D, id_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  if (color_channel_count == 4) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_img, height_img, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
  } else if (color_channel_count == 3) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_img, height_img, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
  } else if (color_channel_count == 1) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_img, height_img, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
  } else {
    throw std::invalid_argument{"Automatic Texture type recognition failed"};
  }
  
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(bytes);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::tex_unit(const Shader& shader, const char* uniform, const GLint unit) {
  const GLint tex_uniform = glGetUniformLocation(shader.get_id(), uniform);
  shader.activate();
  glUniform1i(tex_uniform, unit);
}

GLuint Texture::get_id() const {
  return id_;
}

const char* Texture::get_type() const {
  return type_;
}

GLuint Texture::get_unit() const {
  return unit_;
}

void Texture::bind() const {
  glActiveTexture(GL_TEXTURE0 + unit_);
  glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::unbind() {
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() const {
  glDeleteTextures(1, &id_);
}
