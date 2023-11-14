#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED

#include <glad/glad.h>

#define TEX_TYPE_DIFFUSE "diffuse"
#define TEX_TYPE_SPECULAR "specular"

#include "Shader.hpp"

class Texture {
  GLuint id_;
  const char* type_;
  GLuint unit_;

public:
  Texture(const char* image, const char* tex_type, GLuint slot);
  static void tex_unit(const Shader& shader, const char* uniform, GLint unit);
  GLuint get_id() const;
  const char* get_type() const;
  GLuint get_unit() const;
  void bind() const;
  static void unbind();
  void Delete() const;
};

#endif
