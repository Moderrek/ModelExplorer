#ifndef SHADER_INCLUDED
#define SHADER_INCLUDED

#include <glad/glad.h>
#include <string>
#include <sstream>

std::string get_file_contents(const char* file_name);

class Shader {
	GLuint id_;
public:
	Shader(const char* vertex_file, const char* fragment_file);
	GLuint get_id() const;
	void activate() const;
	void Delete() const;
private:
	static void compile_errors(GLuint shader, const char* type);
};

#endif
