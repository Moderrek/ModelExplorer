#ifndef EBO_INCLUDED
#define EBO_INCLUDED

#include <vector>
#include <glad/glad.h>

class EBO {
  GLuint id_;

public:
  EBO(const std::vector<GLuint>& indices);
  GLuint get_id() const;
  void bind() const;
  static void unbind();
  void Delete() const;
};

#endif
