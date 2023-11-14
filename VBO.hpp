#ifndef VBO_INCLUDED
#define VBO_INCLUDED

#include <vector>
#include <glad/glad.h>

#include "Vertex.hpp"

class VBO {
  GLuint id_;

public:
  explicit VBO(const std::vector<Vertex>& vertices);
  inline GLuint get_id() const;
  void bind() const;
  static void unbind();
  void Delete() const;
};

#endif
