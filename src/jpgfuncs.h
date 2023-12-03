#include <vector>
#include <GLFW/glfw3.h>

namespace jpg {

void draw_at_cursor(double xpos, double ypos, std::vector<GLubyte> &pixs, int wheight, double ratio, int dwidth);

void update_time(double &deltat, double &lastframe);

void randomize_noise_test(std::vector<GLubyte> &pixs);

#ifdef JPGFUNCS_IMPLEMENTATION

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