#ifndef SHADER_INCLUDED
#define SHADER_INCLUDED

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* file_name);

class Shader {
public:
	GLuint ID;
	Shader(const char* vertex_file, const char* fragment_file);
	void Activate();
	void Delete();
};

#endif
