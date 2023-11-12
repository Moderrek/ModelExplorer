#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "vao.hpp"
#include "ebo.hpp"
#include "vbo.hpp"

int main() {
	// Initialize GLFW
	glfwInit();
	// Using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	// Create GLFWwindow object of 800 by 800 pixels, naming it "Model Explorer"
	GLFWwindow* window = glfwCreateWindow(800, 800, "Model Explorer", NULL, NULL);
	// Handle error
	if (window == NULL) {
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	// Load GLAD  so it configures OpenGL
	gladLoadGL();
	// Specify viewport of OpenGL in the Window
	// x = 0, y = 0 to x = 800, y = 800
	glViewport(0, 0, 800, 800);

	// Vertices coordinates
	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
	};

	GLuint indices[] = {
		0, 3, 5, // Lower left triangle
		3, 2, 4, // Lower right triangle
		5, 4, 1 // Upper triangle
	};

	Shader shaderProgram{ "default.vert", "default.frag" };
	VAO vao;
	vao.Bind();
	VBO vbo{ vertices, sizeof(vertices) };
	EBO ebo{ indices, sizeof(indices) };
	vao.LinkVBO(vbo, 0);
	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();

	// Main while loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();
		vao.Bind();
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	vao.Delete();
	vbo.Delete();
	ebo.Delete();
	shaderProgram.Delete();

	// Delete window
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}