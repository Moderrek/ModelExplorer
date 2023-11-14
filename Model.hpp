#ifndef MODEL_HPP_INCLUDED
#define MODEL_HPP_INCLUDED

#include <json/json.hpp>
#include "Mesh.hpp"

using json = nlohmann::json;

class Model {
  const char* file_;
  std::vector<unsigned char> data_;
  json json_;

  std::vector<Mesh> meshes_;
  std::vector<glm::vec3> meshes_translation_;
  std::vector<glm::quat> meshes_rotation_;
  std::vector<glm::vec3> meshes_scale_;
  std::vector<glm::mat4> meshes_matrices_;
  
  std::vector<std::string> loaded_textures_name_;
  std::vector<Texture> loaded_textures_;

  void load_mesh(unsigned int mesh_index);

  void traverse_node(const size_t next_node = 0, const glm::mat4& matrix = glm::mat4{1.0f});
  
  std::vector<unsigned char> get_data();
  std::vector<Texture> get_textures();
  std::vector<float> get_floats(json accessor);
  std::vector<GLuint> get_indexes(json accessor);
  std::vector<glm::vec2> group_floats_vec2(const std::vector<float>& floats);
  std::vector<glm::vec3> group_floats_vec3(const std::vector<float>& floats_vector);
  std::vector<glm::vec4> group_floats_vec4(const std::vector<float>& floats_vector);
  std::vector<Vertex> assemble_vertices(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs);
public:
  explicit Model(const char* file);
  void draw(const Shader& shader, Camera& camera) const;
};

#endif