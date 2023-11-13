#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

GLfloat vertices[] = {
	-1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	-1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
};

GLuint indices[] = {
	0, 1, 2,
	0, 2, 3
};

GLfloat light_vertices[] = {
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint light_indices[] = {
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};


constexpr unsigned int width = 800;
constexpr unsigned int height = 800;

int main() {
	// Initialize GLFW
	glfwInit();
	// Using OpenGL 3.3
	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create GLFWwindow object of 800 by 800 pixels, naming it "Model Explorer"
	GLFWwindow* window = glfwCreateWindow(width, height, "Model Explorer", nullptr, nullptr);
	// Handle error
	if (window == nullptr) {
		std::cerr << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	// Load GLAD  so it configures OpenGL
	gladLoadGL();
	// Specify viewport of OpenGL in the Window
	glViewport(0, 0, width, height);
	Shader shader_program{ "shaders/default.vert", "shaders/default.frag" };
	VAO vao;
	vao.bind();
	VBO vbo{ vertices, sizeof(vertices) };
	EBO ebo{ indices, sizeof(indices) };
	VAO::link_attrib(vbo, 0, 3, GL_FLOAT, 11 * sizeof(float), 0);
	VAO::link_attrib(vbo, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO::link_attrib(vbo, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO::link_attrib(vbo, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	VAO::unbind();
	VBO::unbind();
	EBO::unbind();

	Shader light_shader{ "shaders/light.vert", "shaders/light.frag" };
	VAO light_vao;
	light_vao.bind();

	VBO light_vbo{ light_vertices, sizeof(light_vertices) };
	EBO light_ebo{ light_indices, sizeof(light_indices) };

	VAO::link_attrib(light_vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), nullptr);
	VAO::unbind();
	VBO::unbind();
	EBO::unbind();

	auto light_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	auto light_pos = glm::vec3{0.5f, 0.5f, 0.5f};
	auto light_model = glm::mat4{ 1.0f };
	light_model = translate(light_model, light_pos);

	auto pyramid_pos = glm::vec3{ 0.0f, 0.0f, 0.0f };
	auto pyramid_model = glm::mat4{ 1.0f };
	pyramid_model = translate(pyramid_model, pyramid_pos);

	light_shader.activate();
	glUniformMatrix4fv(glGetUniformLocation(light_shader.get_id(), "model"), 1, GL_FALSE, value_ptr(light_model));
	glUniform4f(glGetUniformLocation(light_shader.get_id(), "lightColor"), light_color.x, light_color.y, light_color.z, light_color.w);
	shader_program.activate();
	glUniformMatrix4fv(glGetUniformLocation(shader_program.get_id(), "model"), 1, GL_FALSE, value_ptr(pyramid_model));
	glUniform4f(glGetUniformLocation(shader_program.get_id(), "lightColor"), light_color.x, light_color.y, light_color.z, light_color.w);
	glUniform3f(glGetUniformLocation(shader_program.get_id(), "lightPos"), light_pos.x, light_pos.y, light_pos.z);

	// Texture
	Texture texture_dirt{ "textures/dirt_color.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE };
	Texture::tex_unit(shader_program, "tex0", 0);
	Texture texture_dirt_spec {"textures/dirt_ao.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE};
	Texture::tex_unit(shader_program, "tex1", 1);

	glEnable(GL_DEPTH_TEST);

	Camera camera{ width, height, glm::vec3{0.0f, 0.0f, 2.0f} };
	
	// Main while loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera.inputs(window);
		camera.update_matrix(45.0f, 0.1f, 100.0f);
		shader_program.activate();
		glUniform3f(glGetUniformLocation(shader_program.get_id(), "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		// Export the camera matrix to the Vertex Shader
		camera.matrix(shader_program, "camMatrix");
		// Binds texture so that is appears in rendering
		texture_dirt.bind();
		texture_dirt_spec.bind();
		// Bind the Vertex Array Object so OpenGl knows to use it
		vao.bind();
		// Draw primitives
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, nullptr);
		// Tells OpenGL which Shader Program we want to use
		light_shader.activate();
		camera.matrix(light_shader, "camMatrix");
		light_vao.bind();
		glDrawElements(GL_TRIANGLES, sizeof(light_indices) / sizeof(int), GL_UNSIGNED_INT, nullptr);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	vao.Delete();
	vbo.Delete();
	ebo.Delete();
	shader_program.Delete();
	texture_dirt.Delete();

	// Delete window
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}