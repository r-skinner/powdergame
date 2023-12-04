#include "jpgfuncs.h"
#include "jpgglobals.h"
#include "config.h"
#include "utils.h"
#include "materials/material.h"




static PGInfo info;
void updateSimulations(std::vector<GLubyte> &pixs, bool &oddf) {

    info.targetOddBit = info.oddFrame ? 0b10000000 : 0b00000000;
    info.nonTargetOddBit = info.oddFrame ? 0b00000000 : 0b10000000;


    for (int y = DISPLAY_HEIGHT - 1; y > 0; y--) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            int i = y * DISPLAY_WIDTH + x;

            GLubyte colorBitsHere = pixs[i] & info.COLOR_BITS;

            if (colorBitsHere != 0b00000000 && (pixs[i] & info.ODD_BIT) == info.targetOddBit
                && i != DISPLAY_WIDTH * (DISPLAY_HEIGHT - 1)) {
                POWDER_FUNCS[colorBitsHere](pixs, i, info, colorBitsHere, densities);
            }
        }
    }
    info.oddFrame = !info.oddFrame;
    oddf = info.oddFrame;
}

void drawAtCursor(std::vector<GLubyte> &pixs, int selectedColor) {
    double yp = WINDOW_HEIGHT - mouse.y;

    int dx = static_cast<int>(mouse.x * RATIO);
    int dy = static_cast<int>(yp * RATIO);

    int index = dy * DISPLAY_WIDTH + dx;

    GLubyte theByte = selectedColor;
    theByte |= info.LIQUID_TRAV_LEFT_BIT;

    if (index != DISPLAY_WIDTH * (DISPLAY_HEIGHT - 1)) {
        pixs[index] = theByte;
    }
}

void updateTime(double &deltat, double &lastframe) {
    double current_frame = glfwGetTime();
    deltat = current_frame - lastframe;
    lastframe = current_frame;
}