#ifndef POWDERGAMEO_JPGFUNCS_H
#define POWDERGAMEO_JPGFUNCS_H

#include <functional>
#include <GL/glew.h>
#include <vector>

void updateSimulations(std::vector<GLubyte> &pixs, bool &oddf);

void drawAtCursor(std::vector <GLubyte> &pixs, int selectedColor);

void updateTime(double &deltat, double &lastframe);

#endif //POWDERGAMEO_JPGFUNCS_H
