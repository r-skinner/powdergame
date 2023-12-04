#ifndef POWDERGAMEO_JPGGLOBALS_H
#define POWDERGAMEO_JPGGLOBALS_H

#include <GLFW/glfw3.h>
#include <map>
#include <vector>
#include <array>
#include <functional>
#include <glm/vec3.hpp>
#include "pginfo.h"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);


extern std::array<std::function<void(std::vector<GLubyte>&, int i, PGInfo&, GLubyte, std::array<GLubyte, 16>)>, 16> POWDER_FUNCS;

extern GLuint SHADER_PROG1;
extern GLuint TEXTURE_ID;

//TIME
extern double DELTA_TIME;
extern double LAST_FRAME;

//MOUSE
extern bool MOUSE_CLICKED;
extern double MOUSEX;
extern double MOUSEY;


//GAME
extern bool ODD_FRAME;
extern std::map<int, int> keyNumMap;

extern int SELECTED_COLOR;

extern std::array<glm::vec3, 16> colorPalette;

extern std::array<GLubyte, 16> densities;


#endif //POWDERGAMEO_JPGGLOBALS_H
