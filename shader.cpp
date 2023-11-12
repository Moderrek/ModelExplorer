#include "shader.hpp"

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
	throw(errno);
}

Shader::Shader(const char* vertex_file, const char* fragment_file) {
	std::string vertex_code = get_file_contents(vertex_file);
	std::string fragment_code = get_file_contents(fragment_file);

	const char* vertex_source = vertex_code.c_str();
	const char* fragment_source = fragment_code.c_str();

	// Create Vertex Shader Object and get reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertex_source, NULL);
	// Compile the Vertex Shader into machine code runtime
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(fragmentShader, 1, &fragment_source, NULL);
	glCompileShader(fragmentShader);

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Link all the shaders together into the Shader Program
	glLinkProgram(ID);
	// Delete useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate() {
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}