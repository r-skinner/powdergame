#include <GLFW/glfw3.h>
#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include "textloader.h"

GLFWwindow* WINDOW;

std::array<glm::vec3, 16> colorPalette = {
    glm::vec3(0.0f, 0.0f, 0.0f),//black 0
    glm::vec3(0.0f, 0.0f, 1.0f),//blue 1
    glm::vec3(0.0f, 1.0f, 0.0f),//green 2
    glm::vec3(1.0f, 1.0f, 0.0f),//yellow 3
    glm::vec3(1.0f, 0.5f, 0.0f),//orange 4
    glm::vec3(0.0f, 1.0f, 1.0f),//cyan 5
    glm::vec3(0.0f, 0.5f, 1.0f),//teal 6
    glm::vec3(1.0f, 0.0f, 1.0f),//magenta 7
    glm::vec3(0.5f, 0.0f, 0.6f),//purple 8
    glm::vec3(0.5f, 0.5f, 0.5f),//grey 9
    glm::vec3(1.0f, 0.0f, 0.0f),//red 10
    glm::vec3(1.0f, 1.0f, 1.0f),//white 11
    glm::vec3(1.0f, 1.0f, 1.0f),//white 12
    glm::vec3(1.0f, 1.0f, 1.0f),//white 13
    glm::vec3(1.0f, 1.0f, 1.0f),//white 14
    glm::vec3(1.0f, 1.0f, 1.0f)//white 15
};

struct PGConfig {
    double ratio;
};

PGConfig load_config() {
    std::string CONFIGTXT;
    load_text("CONFIG.txt", CONFIGTXT);
    std::istringstream sstream(CONFIGTXT);

    PGConfig theConfig;

    std::string word;
    while(sstream >> word) {
        if(word == "Ratio:") {
            sstream >> word;
            theConfig.ratio = std::stod(word);
        }
    }
    return theConfig;
}

int WWIDTH = 1080;
int WHEIGHT = 760;

PGConfig CONFIG = load_config();

double RATIO = CONFIG.ratio;

int DWIDTH = static_cast<int>(WWIDTH * RATIO);
int DHEIGHT = static_cast<int>(WHEIGHT * RATIO);

GLuint SHADER_PROG1;
GLuint TEXTURE_ID;

//TIME
double DELTA_TIME = 0;
double LAST_FRAME = 0;

//MOUSE
bool MOUSE_CLICKED = false;
double MOUSEX = 0;
double MOUSEY = 0;

std::vector<GLubyte> PIXELS(DWIDTH * DHEIGHT);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

#ifdef JPG_CALLBACK_IMPLEMENTATION

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if(action == GLFW_PRESS) {
            MOUSE_CLICKED = true;
        } else if (action == GLFW_RELEASE) {
            MOUSE_CLICKED = false;
        }
    }
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    MOUSEX = xpos;
    MOUSEY = ypos;
}

#endif