#include "App.hpp"

App::App(const int window_width, const int window_height, const char* title): window_(nullptr), shader_program_(nullptr) {
  window_width_ = window_width;
  window_height_ = window_height;
  window_title_ = title;
}

App::~App() {
  delete shader_program_;
  delete shader_outlining_program_;
}

void App::init() {
  // Initialize GLFW
  if (glfwInit() == GL_FALSE) {
    std::cerr << "Cannot init GLFW" << std::endl;
    exit(-1);
  }
  std::cout << "GLFW Initialized." << std::endl;
  // Using OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(window_width_, window_height_, window_title_, nullptr, nullptr);
  if (window_ == nullptr) {
    std::cerr << "Failed to create GLFW window!" << std::endl;
    glfwTerminate();
    exit(-2);
  }
  std::cout << "Successfully create window." << std::endl;
  // Introduce the window into the current context
  glfwMakeContextCurrent(window_);
  // Load GLAD  so it configures OpenGL
  std::cout << "Loading OpenGL.." << std::endl;
  gladLoadGL();
  // Specify viewport of OpenGL in the Window
  std::cout << "Creating viewport " << window_width_ << 'x' << window_height_ << ".." << std::endl;
  glViewport(0, 0, window_width_, window_height_);

  std::cout << "Compiling shaders runtime.." << std::endl;
  try {
    shader_program_ = new Shader{"shaders/default.vert", "shaders/default.frag"};
    // shader_outlining_program_ = new Shader{"shaders/outlining.vert", "shaders/outlining.frag"};
  }  catch (const std::invalid_argument& exception) {
    std::cerr << "Exception: " << exception.what() << std::endl;
    glfwDestroyWindow(window_);
    glfwTerminate();
    exit(-3);
  }

  constexpr auto light_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  constexpr auto light_pos = glm::vec3(0.5f, 10.5f, 0.5f);
  auto light_model = translate(glm::mat4(1.0f), light_pos);
  
  std::cout << "Setup lighting.." << std::endl;
  shader_program_->activate();
  glUniform4f(glGetUniformLocation(shader_program_->get_id(), "lightColor"), light_color.x, light_color.y, light_color.z, light_color.w);
  glUniform3f(glGetUniformLocation(shader_program_->get_id(), "lightPos"), light_pos.x, light_pos.y, light_pos.z);

  std::cout << "Enabling depth buffer.." << std::endl;
  glEnable(GL_DEPTH_TEST);
  std::cout << "Enabling stencil buffer.." << std::endl;
  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  std::cout << "Enabling cull face.." << std::endl;
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glFrontFace(GL_CW);
  
  std::cout << "Successfully initialized." << std::endl;
}

void App::run() const {
  std::cout << "Creating camera.." << std::endl;
  Camera camera{window_width_, window_height_, glm::vec3{0.0f, 0.0f, 2.0f}};
  std::cout << "Loading external model.." << std::endl;
  Model ground {"models/ground/scene.gltf"};
  Model trees {"models/trees/scene.gltf"};
  Model sword {"models/sword/scene.gltf"};
  
  std::cout << "Successfully started main loop." << std::endl;
  // Main while loop
  while (!glfwWindowShouldClose(window_)) {
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    // Clean back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // Handle camera inputs
    camera.inputs(window_);
    // Updates and exports the camera  matrix to the Vertex Shader
    camera.update_matrix(80.0f, 0.1f, 250.0f);
    // Draw meshes
    sword.draw(*shader_program_, camera);
    ground.draw(*shader_program_, camera);
    trees.draw(*shader_program_, camera);
    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window_);
    glfwPollEvents();
  }
  std::cout << "Deleting shaders.." << std::endl;
  shader_program_->Delete();
  shader_outlining_program_->Delete();
  std::cout << "Deleting window.." << std::endl;
  glfwDestroyWindow(window_);
  glfwTerminate();
}
