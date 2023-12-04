#include "utils.h"
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

mouse_t mouse;
GLubyte SELECTED_COLOR = 1;

// STATIC DEFINITIONS
int NUM_BAR_0 = 48;
int NUM_BAR_9 = 59;

float QUAD_VERTS[] = {
        // positions   // texture coords
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
};
int QUAD_VERTS_LEN = sizeof(QUAD_VERTS);

int load_text(const char *fp, std::string &out) {
    std::ifstream text(fp);
    if (!text.is_open()) { return -1; }
    std::stringstream text_stream;
    text_stream << text.rdbuf();
    text.close();
    out = text_stream.str();
    return 1;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        mouse.left_click = (action == GLFW_PRESS);
    }
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
    mouse.x = xpos;
    mouse.y = ypos;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key >= GLFW_KEY_0 & key <= GLFW_KEY_9) {
        SELECTED_COLOR = key - GLFW_KEY_0;
    }
}

void load_icon(GLFWwindow *window, const std::string& filename) {
    int width, height, channels;
    unsigned char *iconData = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (!iconData) {
        fprintf(stderr, "Failed to load icon: %s", stbi_failure_reason());
    }
    if (iconData) {
        GLFWimage icon = {width, height, iconData};
        glfwSetWindowIcon(window, 1, &icon);
        stbi_image_free(iconData);
    }
}
