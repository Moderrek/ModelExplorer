﻿#ifndef VERTEX_HPP_INCLUDED
#define VERTEX_HPP_INCLUDED

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec3 color;
  glm::vec2 tex_uv;
};

#endif