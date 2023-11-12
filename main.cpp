#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "vao.hpp"
#include "vbo.hpp"
#include "ebo.hpp"

// Vertices coordinates
GLfloat vertices[] =
{ //               COORDINATES                  /     COLORS           //
	-0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower left corner
	 0.5f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f, // Lower right corner
	 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,     1.0f, 0.6f,  0.32f, // Upper corner
	-0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner left
	 0.25f, 0.5f * float(sqrt(3)) * 1 / 6, 0.0f,     0.9f, 0.45f, 0.17f, // Inner right
	 0.0f, -0.5f * float(sqrt(3)) * 1 / 3, 0.0f,     0.8f, 0.3f,  0.02f  // Inner down
};

// Indices for vertices order
GLuint indices[] =
{
	0, 3, 5, // Lower left triangle
	3, 2, 4, // Lower right triangle
	5, 4, 1 // Upper triangle
};

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
	Shader shaderProgram{ "default.vert", "default.frag" };
	VAO vao;
	vao.Bind();
	VBO vbo{ vertices, sizeof(vertices) };
	EBO ebo{ indices, sizeof(indices) };
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*) 0);
	vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	std::cout << "Hello!" << std::endl;
	// Main while loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.Activate();
		glUniform1f(uniID, 0.5f);
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