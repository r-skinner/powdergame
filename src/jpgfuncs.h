#include <vector>
#include <GLFW/glfw3.h>
#include <iostream>

namespace jpg {

void draw_at_cursor(double xpos, double ypos, std::vector<GLubyte> &pixs, int wheight, double ratio, int dwidth);

void update_time(double &deltat, double &lastframe);

void randomize_noise_test(std::vector<GLubyte> &pixs);

void UPDATE_SIMULATION(std::vector<GLubyte> &pixs, int dwidth, int dheight);






















#ifdef JPGFUNCS_IMPLEMENTATION

void UPDATE_SIMULATION(std::vector<GLubyte> &pixs, int dwidth, int dheight) {
    static bool oddFrame = false;

    static GLubyte oddBit = 0b10000000;
    static GLubyte clearOddBit = 0b01111111;

    static GLubyte colorBits = 0b00001111;
    static GLubyte clearColorBits = 0b11110000;

    GLubyte targetOddBit = oddFrame ? 0b10000000 : 0b00000000;
    GLubyte nonTargetOddBit = oddFrame ? 0b00000000 : 0b10000000;
    
    //std::cout << "up " << (oddFrame ? "t" : "f") << '\n';

    for(int y = dheight-1; y > 0; y--) {
        for(int x = 0; x < dwidth; x++) {
            int i = y * dwidth + x;

            GLubyte colorBitsHere = pixs[i] & colorBits;

            if(colorBitsHere != 0b00000000 && y > 0 && (pixs[i] & oddBit) == targetOddBit) {
                int bottomi = i - dwidth;
                int bottomli = bottomi - 1;
                int bottomri = bottomi + 1;
                if((pixs[bottomi] & colorBits) == 0) {
                    pixs[i] &= clearColorBits;
                    pixs[bottomi] |= colorBitsHere;
                    pixs[bottomi] &= clearOddBit;
                    pixs[bottomi] |= nonTargetOddBit;
                } else {
                    if(oddFrame) {
                        if((pixs[bottomri] & colorBits) == 0) {
                            pixs[i] &= clearColorBits;
                            pixs[bottomri] |= colorBitsHere;
                            pixs[bottomri] &= clearOddBit;
                            pixs[bottomri] |= nonTargetOddBit;
                        }
                    } else {
                        if((pixs[bottomli] & colorBits) == 0) {
                            pixs[i] &= clearColorBits;
                            pixs[bottomli] |= colorBitsHere;
                            pixs[bottomli] &= clearOddBit;
                            pixs[bottomli] |= nonTargetOddBit;
                        }
                    }
                }
            }
        }
    }
    oddFrame = !oddFrame;
}

void draw_at_cursor(double xpos, double ypos, std::vector<GLubyte> &pixs, int wheight, double ratio, int dwidth) {
    int yp = wheight - static_cast<int>(ypos);

    int dx, dy;
    dx = static_cast<int>(xpos*ratio);
    dy = static_cast<int>(yp*ratio);

    int index = dy * dwidth + dx;
    pixs[index] = 1;
}

void update_time(double &deltat, double &lastframe) {
    double current_frame = glfwGetTime();
    deltat = current_frame - lastframe;
    lastframe = current_frame;
}

void randomize_noise_test(std::vector<GLubyte> &pixs) {
    for(GLubyte &a : pixs) {
        a = static_cast<GLubyte>(rand() % 3);
    }
}

#endif

}