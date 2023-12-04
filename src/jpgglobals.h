#ifndef POWDERGAMEO_JPGGLOBALS_H
#define POWDERGAMEO_JPGGLOBALS_H

#include <GLFW/glfw3.h>
#include <map>
#include <vector>
#include <array>
#include <functional>
#include <glm/vec3.hpp>
#include "materials/material.h"


extern std::array<std::function<void(std::vector<GLubyte>&, int i, PGInfo&, GLubyte, std::array<GLubyte, 16>)>, 16> POWDER_FUNCS;

//TIME
extern double DELTA_TIME;
extern double LAST_FRAME;


//GAME
extern bool ODD_FRAME;

//extern int SELECTED_COLOR;

extern std::array<glm::vec3, 16> colorPalette;

extern std::array<GLubyte, 16> densities;


#endif //POWDERGAMEO_JPGGLOBALS_H
