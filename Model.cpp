#include "Model.hpp"

#include <iostream>

void Model::load_mesh(const unsigned int mesh_index) {
  auto primitives = json_["meshes"][mesh_index]["primitives"][0];
  const unsigned int pos_index = primitives["attributes"]["POSITION"];
  const unsigned int normal_index = primitives["attributes"]["NORMAL"];
  const unsigned int uv_index = primitives["attributes"]["TEXCOORD_0"];
  const unsigned int indexes_index = primitives["indices"];

  auto accessors = json_["accessors"];
  
  const auto vector_positions = get_floats(accessors[pos_index]);
  const auto positions = group_floats_vec3(vector_positions);

  const auto vector_normal = get_floats(accessors[normal_index]);
  const auto normals = group_floats_vec3(vector_normal);

  const auto vector_uv = get_floats(accessors[uv_index]);
  const auto uvs = group_floats_vec2(vector_uv);

  auto vertices = assemble_vertices(positions, normals, uvs);
  auto indexes = get_indexes(accessors[indexes_index]);
  auto textures = get_textures();

  meshes_.emplace_back(vertices, indexes, textures);
}

void Model::traverse_node(const size_t next_node, const glm::mat4& matrix) {
  auto node = json_["nodes"][next_node];

  auto translation = glm::vec3{0.0f, 0.0f, 0.0f};
  if (node.find("translation") != node.end()) {
    float translation_values[3];
    for (unsigned int i = 0; i < node["translation"].size(); i++) {
      translation_values[i] = node["translation"][i];
    }
    translation = glm::make_vec3(translation_values);
  }

  auto rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  if (node.find("rotation") != node.end()) {
    const float rot_values[4] {
      node["rotation"][3],
      node["rotation"][0],
      node["rotation"][1],
      node["rotation"][2]
    };
    rotation = glm::make_quat(rot_values);
  }

  auto scale = glm::vec3{1.0f, 1.0f, 1.0f};
  if (node.find("scale") != node.end()) {
    float scale_values[3];
    for (unsigned int i = 0; i < node["scale"].size(); i++) {
      scale_values[i] = node["scale"][i];
    }
    scale = glm::make_vec3(scale_values);
  }

  auto mat_node = glm::mat4{1.0f};
  if (node.find("matrix") != node.end()) {
    float matrix_values[16];
    for (unsigned int i = 0; i < node["matrix"].size(); i++) {
      matrix_values[i] = node["matrix"][i];
    }
    mat_node = glm::make_mat4(matrix_values);
  }
  
  auto trans = glm::mat4(1.0f);
  auto sca = glm::mat4(1.0f);
  trans = translate(trans, translation);
  const auto rot = mat4_cast(rotation);
  sca = glm::scale(sca, scale);
  
  const glm::mat4 mat_next_node = matrix * mat_node * trans * rot * sca;
  
  if (node.find("mesh") != node.end()) {
    meshes_translation_.push_back(translation);
    meshes_rotation_.push_back(rotation);
    meshes_scale_.push_back(scale);
    meshes_matrices_.push_back(mat_next_node);
    load_mesh(node["mesh"]);
  }

  if (node.find("children") != node.end()) {
    for (size_t i = 0; i < node["children"].size(); i++) traverse_node(node["children"][i], mat_next_node);
  }
}

std::vector<unsigned char> Model::get_data() {
  const std::string uri = json_["buffers"][0]["uri"];
  const auto file_str = std::string(file_);
  const std::string file_directory = file_str.substr(0, file_str.find_last_of('/') + 1);
  std::string bytes_text = get_file_contents((file_directory + uri).c_str());
  return {bytes_text.begin(), bytes_text.end()};
}

std::vector<Texture> Model::get_textures() {
  std::vector<Texture> textures;
  const auto file_str = std::string(file_);
  const std::string file_directory = file_str.substr(0, file_str.find_last_of('/') + 1);
  for (unsigned int i = 0; i < json_["images"].size(); i++) {
    std::string texture_path = json_["images"][i]["uri"];
    bool skip = false;
    for (size_t j = 0; j < loaded_textures_name_.size(); j++) {
      if (loaded_textures_name_[j] == texture_path) {
        textures.push_back(loaded_textures_[j]);
        skip = true;
        break;
      }
    }
    if (skip) continue;
    if (texture_path.find("baseColor") != std::string::npos || texture_path.find("diffuse") != std::string::npos) {
      auto diffuse = Texture{(file_directory + texture_path).c_str(), TEX_TYPE_DIFFUSE, unsigned(loaded_textures_.size())};
      textures.push_back(diffuse);
      loaded_textures_.push_back(diffuse);
      loaded_textures_name_.push_back(texture_path);
      std::cout << "Loaded color texture " << file_ << texture_path << std::endl;
    } else if (texture_path.find("metallicRoughness") != std::string::npos) {
      auto specular = Texture{(file_directory + texture_path).c_str(), TEX_TYPE_SPECULAR, unsigned(loaded_textures_.size())};
      textures.push_back(specular);
      loaded_textures_.push_back(specular);
      loaded_textures_name_.push_back(texture_path);
      std::cout << "Loaded metallic/roughness texture " << file_ << texture_path << std::endl;
    }
  }
  return textures;
}

std::vector<float> Model::get_floats(json accessor) {
  std::vector<float> floats;

  const unsigned int buffer_view_ind = accessor.value("bufferView", 1);
  const unsigned int count = accessor["count"];
  const unsigned int acc_byte_offset = accessor.value("byteOffset", 0);
  const std::string type = accessor["type"];
  json buffer_view = json_["bufferViews"][buffer_view_ind];
  const unsigned int byte_offset = buffer_view["byteOffset"];

  unsigned int num_per_vert;
  if (type == "SCALAR") {
    num_per_vert = 1;
  } else if (type == "VEC2") {
    num_per_vert = 2;
  } else if (type == "VEC3") {
    num_per_vert = 3;
  } else if (type == "VEC4") {
    num_per_vert = 4;
  } else {
    throw std::invalid_argument{"Type is invalid (not SCALAR, VEC2, VEC3, VEC4)"};
  }

  const unsigned int beginning_of_data = byte_offset + acc_byte_offset;
  const unsigned int length_of_data = count * 4 * num_per_vert;
  for (unsigned int i = beginning_of_data; i < beginning_of_data + length_of_data; i += 4) {
    const unsigned char bytes[] = { data_[i], data_[i+1], data_[i+2], data_[i+3] };
    float value;
    std::memcpy(&value, bytes, sizeof(float));
    floats.push_back(value);
  }
  
  return floats;
}

std::vector<GLuint> Model::get_indexes(json accessor) {
  std::vector<GLuint> indexes;

  const unsigned int buffer_view_index = accessor.value("bufferView", 1);
  const unsigned int count = accessor["count"];
  const unsigned int acc_byte_offset = accessor.value("byteOffset", 0);
  const unsigned int type = accessor["componentType"];
  json buffer_view = json_["bufferViews"][buffer_view_index];
  const unsigned int byte_offset = buffer_view["byteOffset"];

  const unsigned int beginning_of_data = byte_offset + acc_byte_offset;
  if (type == 5125) {
    for (unsigned int i = beginning_of_data; i < byte_offset + acc_byte_offset + count * 4; i += 4) {
      const unsigned char bytes[] = { data_[i], data_[i+1], data_[i+2], data_[i+3] };
      unsigned int value;
      std::memcpy(&value, bytes, sizeof(unsigned int));
      indexes.push_back(value);
    }
  } else if (type == 5123) {
    for (unsigned int i = beginning_of_data; i < byte_offset + acc_byte_offset + count * 2; i += 2) {
      const unsigned char bytes[] = { data_[i], data_[i+1] };
      unsigned short value;
      std::memcpy(&value, bytes, sizeof(unsigned short));
      indexes.push_back(value);
    }
  } else if (type == 5122) {
    for (unsigned int i = beginning_of_data; i < byte_offset + acc_byte_offset + count * 2; i += 2) {
      const unsigned char bytes[] = { data_[i], data_[i+1] };
      short value;
      std::memcpy(&value, bytes, sizeof(short));
      indexes.push_back(value);
    }
  }
  
  return indexes;
}

std::vector<glm::vec2> Model::group_floats_vec2(const std::vector<float>& floats) {
  std::vector<glm::vec2> vectors;
  for (size_t i = 0; i < floats.size(); i += 2) vectors.emplace_back(floats[i], floats[i+1]);
  return vectors;
}

std::vector<glm::vec3> Model::group_floats_vec3(const std::vector<float>& floats) {
  std::vector<glm::vec3> vectors;
  for (size_t i = 0; i < floats.size(); i += 3) vectors.emplace_back(floats[i], floats[i+1], floats[i+2]);
  return vectors;
}

std::vector<glm::vec4> Model::group_floats_vec4(const std::vector<float>& floats) {
  std::vector<glm::vec4> vectors;
  for (size_t i = 0; i < floats.size(); i += 4) vectors.emplace_back(floats[i], floats[i+1], floats[i+2], floats[i+3]);
  return vectors;
}

std::vector<Vertex> Model::assemble_vertices(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals,
                                             const std::vector<glm::vec2>& uvs) {
  std::vector<Vertex> vertices;
  for (size_t i = 0; i < positions.size(); i++) {
    vertices.push_back(Vertex{positions[i], normals[i], glm::vec3{1.0f, 1.0f, 1.0f}, uvs[i]});
  }
  return vertices;
}

Model::Model(const char* file): file_(file) {
  if (!is_file_exists(file)) throw std::invalid_argument{std::string{"Cannot find model .gltf file: "} + file};
  std::string text = get_file_contents(file);
  json_ = json::parse(text);
  data_ = get_data();
  traverse_node();
}

void Model::draw(const Shader& shader, Camera& camera) const {
  for (unsigned int i = 0; i < meshes_.size(); i++) {
    meshes_[i].draw(shader, camera, meshes_matrices_[i]);
  }
}
