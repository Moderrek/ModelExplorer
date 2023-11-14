#include "Shader.hpp"

#include <fstream>
#include <iostream>

std::string get_file_contents(const char* file_name) {
  std::ifstream in(file_name, std::ios::binary);
  if (in) {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    return (contents);
  }
  throw errno;
}

inline bool is_file_exists(const char* file_name) {
  struct stat buffer;
  return stat (file_name, &buffer) == 0;
}

Shader::Shader(const char* vertex_file, const char* fragment_file) {
  if (!is_file_exists(vertex_file)) {
    throw std::invalid_argument{std::string{"Cannot find vertex shader file: "} + vertex_file};
  }
  if (!is_file_exists(fragment_file)) {
    throw std::invalid_argument{std::string{"Cannot find fragment shader file: "} + vertex_file};
  }
  
  const std::string vertex_code = get_file_contents(vertex_file);
  const std::string fragment_code = get_file_contents(fragment_file);

  const char* vertex_source = vertex_code.c_str();
  const char* fragment_source = fragment_code.c_str();

  // Create Vertex Shader Object and get reference
  const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  // Attach Vertex Shader source to the Vertex Shader Object
  glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
  // Compile the Vertex Shader into machine code runtime
  glCompileShader(vertex_shader);
  compile_errors(vertex_shader, "VERTEX");

  // Create Fragment Shader Object and get its reference
  const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
  glCompileShader(fragment_shader);
  compile_errors(fragment_shader, "FRAGMENT");

  id_ = glCreateProgram();
  glAttachShader(id_, vertex_shader);
  glAttachShader(id_, fragment_shader);
  // Link all the shaders together into the Shader Program
  glLinkProgram(id_);
  compile_errors(id_, "PROGRAM");
  // Delete useless Vertex and Fragment Shader objects
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

GLuint Shader::get_id() const {
  return id_;
}

void Shader::activate() const {
  glUseProgram(id_);
}

void Shader::Delete() const {
  glDeleteProgram(id_);
}

void Shader::compile_errors(const GLuint shader, const char* type) {
  GLint has_compiled;
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &has_compiled);
    if (has_compiled == GL_FALSE) {
      char info_log[1024];
      glGetShaderInfoLog(shader, 1024, nullptr, info_log);
      std::cout << "SHADER_COMPILATION_ERROR for: " << type << '\n' << std::endl;
      std::cout << "Shader " << type << ": " << info_log << std::endl;
    }
    else {
      std::cout << "Shader " << type << " compiled" << std::endl;
    }
  }
  else {
    glGetProgramiv(shader, GL_COMPILE_STATUS, &has_compiled);
    if (has_compiled == GL_FALSE) {
      char info_log[1024];
      glGetProgramInfoLog(shader, 1024, nullptr, info_log);
      std::cout << "SHADER_LINKING_ERROR for: " << type << '\n' << std::endl;
      std::cout << "Shader " << type << ": " << info_log << std::endl;
    }
    else {
      std::cout << "Shader " << type << " compiled" << std::endl;
    }
  }
}
