#include "App.hpp"

#include <stb/stb_image.h>

/*
 * glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); -> wireframe
 * glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); -> shaded
 */

float rectangle_vertices[] = {
  1.0f, -1.0f, 1.0f, 0.0f,
  -1.0f, -1.0f, 0.0f, 0.0f,
  -1.0f, 1.0f, 0.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, -1.0f, 1.0f, 0.0f,
  -1.0f, 1.0f, 0.0f, 1.0f
};

float skybox_vertices[] =
{
  //   Coordinates
  -1.0f, -1.0f,  1.0f,//        7--------6
   1.0f, -1.0f,  1.0f,//       /|       /|
   1.0f, -1.0f, -1.0f,//      4--------5 |
  -1.0f, -1.0f, -1.0f,//      | |      | |
  -1.0f,  1.0f,  1.0f,//      | 3------|-2
   1.0f,  1.0f,  1.0f,//      |/       |/
   1.0f,  1.0f, -1.0f,//      0--------1
  -1.0f,  1.0f, -1.0f
};

unsigned int skybox_indices[] =
{
  // Right
  1, 2, 6,
  6, 5, 1,
  // Left
  0, 4, 7,
  7, 3, 0,
  // Top
  4, 5, 6,
  6, 7, 4,
  // Bottom
  0, 3, 2,
  2, 1, 0,
  // Back
  0, 1, 5,
  5, 4, 0,
  // Front
  3, 7, 6,
  6, 2, 3
};

App::App(const int window_width, const int window_height, const char* title): window_(nullptr),
                                                                              shader_program_(nullptr),
                                                                              shader_outlining_program_(nullptr),
                                                                              shader_grass_program_(nullptr),
                                                                              shader_frame_buffer_program_(nullptr) {
  window_width_ = window_width;
  window_height_ = window_height;
  window_title_ = title;
}

App::~App() {
  delete shader_program_;
  delete shader_outlining_program_;
  delete shader_grass_program_;
  delete shader_frame_buffer_program_;
  delete shader_skybox_;
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
    shader_outlining_program_ = new Shader{"shaders/outlining.vert", "shaders/outlining.frag"};
    shader_grass_program_ = new Shader{"shaders/grass.vert", "shaders/grass.frag"};
    shader_frame_buffer_program_ = new Shader{"shaders/framebuffer.vert", "shaders/framebuffer.frag"};
    shader_skybox_ = new Shader{"shaders/skybox.vert", "shaders/skybox.frag"};
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
  shader_grass_program_->activate();
  glUniform4f(glGetUniformLocation(shader_grass_program_->get_id(), "lightColor"), light_color.x, light_color.y, light_color.z, light_color.w);
  glUniform3f(glGetUniformLocation(shader_grass_program_->get_id(), "lightPos"), light_pos.x, light_pos.y, light_pos.z);
  shader_frame_buffer_program_->activate();
  glUniform1i(glGetUniformLocation(shader_frame_buffer_program_->get_id(), "screenTexture"), 0);
  shader_skybox_->activate();
  glUniform1i(glGetUniformLocation(shader_skybox_->get_id(), "skybox"), 0);

  std::cout << "Enabling depth buffer.." << std::endl;
  glEnable(GL_DEPTH_TEST);
  std::cout << "Enabling stencil buffer.." << std::endl;
  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  std::cout << "Enabling cull face.." << std::endl;
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glFrontFace(GL_CCW);

  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// glEnable GL_BLEND glDisable - transparent objects (blending)
  
  std::cout << "Successfully initialized." << std::endl;
}

void App::run() const {
  std::cout << "Creating camera.." << std::endl;
  Camera camera{window_width_, window_height_, glm::vec3{0.0f, 0.0f, 2.0f}};
  std::cout << "Loading external model.." << std::endl;
  Model ground {"models/ground/scene.gltf"};
  Model grass {"models/grass/scene.gltf"};
  
  double previous_time = 0;
  double current_time = 0;
  double timer = 0;
  unsigned int counter = 0;
  double previous_time_delta = 0;

  // Disable V-Sync
  // glfwSwapInterval(0);

  // Enable V-Sync
  glfwSwapInterval(1);
  
  unsigned int rect_vao, rect_vbo;
  glGenVertexArrays(1, &rect_vao);
  glGenBuffers(1, &rect_vbo);
  glBindVertexArray(rect_vao);
  glBindBuffer(GL_ARRAY_BUFFER, rect_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), &rectangle_vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  
  unsigned int fbo;
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  // Create Framebuffer Texture
  unsigned int framebufferTexture;
  glGenTextures(1, &framebufferTexture);
  glBindTexture(GL_TEXTURE_2D, framebufferTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width_, window_height_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

  // Create Render Buffer Object
  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window_width_, window_height_);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);


  // Error checking framebuffer
  auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "Framebuffer error: " << fboStatus << std::endl;
  
  std::cout << "Successfully started main loop." << std::endl;
  // Main while loop
  while (!glfwWindowShouldClose(window_)) {
    current_time = glfwGetTime();
    timer = current_time - previous_time;
    counter += 1;
    if (timer >= 1.0 / 30.0) {
      std::string fps = std::to_string((1.0 / timer) * counter);
      std::string ms = std::to_string((timer / counter) * 1000);
      std::string new_title = "Model Explorer - " + fps + " FPS / " + ms + " ms";
      glfwSetWindowTitle(window_, new_title.c_str());
      previous_time = current_time;
      counter = 0;
    }
    float delta_time = static_cast<float>(current_time) - static_cast<float>(previous_time_delta);
    previous_time_delta = current_time;

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    // Clean back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    // Handle camera inputs
    camera.inputs(window_, delta_time);
    // Updates and exports the camera  matrix to the Vertex Shader
    camera.update_matrix(45.0f, 0.1f, 250.0f);
    // Draw meshes
    glDisable(GL_CULL_FACE);
    ground.draw(*shader_program_, camera);
    grass.draw(*shader_grass_program_, camera);
    glEnable(GL_CULL_FACE);

    // Bind the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Draw the framebuffer rectangle
    shader_frame_buffer_program_->activate();
    glBindVertexArray(rect_vao);
    glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    
    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window_);
    glfwPollEvents();
  }
  std::cout << "Deleting shaders.." << std::endl;
  shader_program_->Delete();
  shader_outlining_program_->Delete();
  shader_grass_program_->Delete();
  shader_frame_buffer_program_->Delete();
  shader_skybox_->Delete();

  // glDeleteFramebuffers(1, &frame_buffer);
  std::cout << "Deleting window.." << std::endl;
  glfwDestroyWindow(window_);
  glfwTerminate();
}
