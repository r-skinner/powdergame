#pragma once
#include <GLFW/glfw3.h>

struct PGInfo {
    static const GLubyte ODD_BIT = 0b10000000;
    static const GLubyte CLEAR_ODD_BIT = 0b01111111;

    static const GLubyte LIQUID_TRAV_LEFT_BIT = 0b01000000;
    static const GLubyte CLEAR_TRAV_LEFT_BIT = 0b10111111;

    static const GLubyte COLOR_BITS = 0b00001111;
    static const GLubyte CLEAR_COLOR_BITS = 0b11110000;

    static const GLubyte FLAG_BITS = 0b11110000;
    static const GLubyte CLEAR_FLAG_BITS = 0b00001111;

    bool oddFrame = false;

    GLubyte targetOddBit;
    GLubyte nonTargetOddBit;
};
