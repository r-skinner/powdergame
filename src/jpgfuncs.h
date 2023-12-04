#ifndef POWDERGAMEO_JPGFUNCS_H
#define POWDERGAMEO_JPGFUNCS_H

#include <functional>
#include "pginfo.h"

void UPDATE_SIMULATION(std::vector<GLubyte> &pixs, int dwidth, int dheight, std::array<std::function<void(std::vector<GLubyte>&, int i, PGInfo&, GLubyte, std::array<GLubyte, 16>)>, 16> &powderFuncs, bool &oddf, std::array<GLubyte, 16>& densities);

void draw_at_cursor(double xpos, double ypos, std::vector<GLubyte> &pixs, int wheight, double ratio, int dwidth, int dheight, int selectedColor, bool oddf);

void update_time(double &deltat, double &lastframe);

#endif //POWDERGAMEO_JPGFUNCS_H
