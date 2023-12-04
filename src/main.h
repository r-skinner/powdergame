#ifndef POWDERGAMEO_MAIN_H
#define POWDERGAMEO_MAIN_H

#include <vector>
#include <GLFW/glfw3.h>

void draw(GLFWwindow *window, std::vector<GLubyte> &PIXELS);
void update(GLFWwindow *window, std::vector<GLubyte> &PIXELS);
int main();

#endif //POWDERGAMEO_MAIN_H
