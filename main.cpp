#include <iostream>

#include "App.hpp"

int main(int argc, char** argv) {
  std::cout << "Model Explorer" << std::endl;
  std::cout << "Working directory: " << argv[0] << std::endl;
  std::cout << "Created by: Tymon Wozniak (https://github.com/Moderrek)" << std::endl;
  App app {800, 800, "Model Explorer"};
  app.init();
  app.run();
  return EXIT_SUCCESS;
}