#ifndef POWDERGAMEO_UTILS_H
#define POWDERGAMEO_UTILS_H

#include <string>
#include <GLFW/glfw3.h>

typedef struct  {
    double x;
    double y;
    bool left_click;
} mouse_t;

extern mouse_t mouse;

extern int NUM_BAR_0;
extern int NUM_BAR_9;

extern float QUAD_VERTS[];
extern int QUAD_VERTS_LEN;

extern GLubyte SELECTED_COLOR;

int load_text(const char *fp, std::string &out);

void load_icon(GLFWwindow* window, const std::string& filename);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

#endif //POWDERGAMEO_UTILS_H
