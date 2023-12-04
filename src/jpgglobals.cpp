#include "config.h"
#include "jpgglobals.h"

GLuint SHADER_PROG1;
GLuint TEXTURE_ID;
double DELTA_TIME = 0;
double LAST_FRAME = 0;
bool MOUSE_CLICKED = false;
double MOUSEX = 0;
double MOUSEY = 0;
bool ODD_FRAME = false;
std::map<int, int> keyNumMap = {
        {GLFW_KEY_1, 1},
        {GLFW_KEY_2, 2},
        {GLFW_KEY_3, 3},
        // {GLFW_KEY_4, 4},
        // {GLFW_KEY_5, 5},
        // {GLFW_KEY_6, 6},
        // {GLFW_KEY_7, 7},
        // {GLFW_KEY_8, 8},
        // {GLFW_KEY_9, 9}
};
int SELECTED_COLOR = 1;
std::array<GLubyte, 16> densities = {
        0,
        100,
        3,
        254,

        0,
        0,
        0,
        0,

        0,
        0,
        0,
        0,

        0,
        0,
        0,
        0
};
std::array<glm::vec3, 16> colorPalette = {
        glm::vec3(0.0f, 0.0f, 0.0f),//black 0
        glm::vec3(0.6f, 0.5f, 0.3f),//tan 1
        glm::vec3(0.0f, 0.0f, 1.0f),//blue 2
        glm::vec3(0.5f, 0.5f, 0.5f),//grey 3
        glm::vec3(1.0f, 0.5f, 0.0f),//orange 4
        glm::vec3(0.0f, 1.0f, 1.0f),//cyan 5
        glm::vec3(0.0f, 0.5f, 1.0f),//teal 6
        glm::vec3(1.0f, 0.0f, 1.0f),//magenta 7
        glm::vec3(0.5f, 0.0f, 0.6f),//purple 8
        glm::vec3(1.0f, 1.0f, 0.0f),//yellow 9
        glm::vec3(1.0f, 0.0f, 0.0f),//red 10
        glm::vec3(0.0f, 1.0f, 0.0f),//green 11
        glm::vec3(1.0f, 1.0f, 1.0f),//white 12
        glm::vec3(1.0f, 1.0f, 1.0f),//white 13
        glm::vec3(1.0f, 1.0f, 1.0f),//white 14
        glm::vec3(1.0f, 1.0f, 1.0f)//white 15
};

std::array<std::function<void(std::vector<GLubyte>&, int i, PGInfo&, GLubyte, std::array<GLubyte, 16>)>, 16> POWDER_FUNCS = {
        [](std::vector<GLubyte>& pixs, int i, PGInfo& info, GLubyte cbh, std::array<GLubyte, 16> densities){
            //placeholder for empty, this never will be called
        },
        /*SAND*/[](std::vector<GLubyte>& pixs, int i, PGInfo& info, GLubyte cbh, std::array<GLubyte, 16> densities){
            int bottomi = i - DISPLAY_WIDTH;
            int bottomli = bottomi - 1;
            int bottomri = bottomi + 1;
            GLubyte colorBitsUnder = (pixs[bottomi] & info.COLOR_BITS);
            if(colorBitsUnder == 0) {
                pixs[i] &= info.CLEAR_COLOR_BITS;
                pixs[bottomi] |= cbh;
                pixs[bottomi] &= info.CLEAR_ODD_BIT;
                pixs[bottomi] |= info.nonTargetOddBit;
            } else {
                if(densities[colorBitsUnder] < densities[cbh]) {
                    GLubyte byteUnder = pixs[bottomi];
                    pixs[bottomi] = pixs[i];
                    pixs[i] = byteUnder;

                    pixs[bottomi] &= info.CLEAR_ODD_BIT;
                    pixs[bottomi] |= info.nonTargetOddBit;
                    pixs[i] &= info.CLEAR_ODD_BIT;
                    pixs[i] |= info.nonTargetOddBit;
                } else {
                    if(info.oddFrame) {
                        if((pixs[bottomri] & info.COLOR_BITS) == 0) {
                            pixs[i] &= info.CLEAR_COLOR_BITS;
                            pixs[bottomri] |= cbh;
                            pixs[bottomri] &= info.CLEAR_ODD_BIT;
                            pixs[bottomri] |= info.nonTargetOddBit;
                        }
                    } else {
                        if((pixs[bottomli] & info.COLOR_BITS) == 0) {
                            pixs[i] &= info.CLEAR_COLOR_BITS;
                            pixs[bottomli] |= cbh;
                            pixs[bottomli] &= info.CLEAR_ODD_BIT;
                            pixs[bottomli] |= info.nonTargetOddBit;
                        }
                    }
                }

            }
        },
        /*WATER*/[](std::vector<GLubyte>& pixs, int i, PGInfo& info, GLubyte cbh, std::array<GLubyte, 16> densities){
            int bottomi = i - DISPLAY_WIDTH;
            int r = i + 1;
            int l = i - 1;
            GLubyte flagBitsHere = pixs[i] & info.FLAG_BITS;
            GLubyte colorBitsUnder = (pixs[bottomi] & info.COLOR_BITS);
            if(colorBitsUnder == 0) {
                pixs[i] &= info.CLEAR_COLOR_BITS;
                pixs[bottomi] |= cbh;
                pixs[bottomi] &= info.CLEAR_FLAG_BITS;

                flagBitsHere &= info.CLEAR_ODD_BIT;
                flagBitsHere |= info.nonTargetOddBit;

                pixs[bottomi] &= info.CLEAR_ODD_BIT;
                pixs[bottomi] |= info.nonTargetOddBit;
                pixs[i] &= info.CLEAR_ODD_BIT;
                pixs[i] |= info.nonTargetOddBit;
            } else {
                if(pixs[i] & info.LIQUID_TRAV_LEFT_BIT) {
                    if ((pixs[l] & info.COLOR_BITS) == 0) {
                        pixs[i] &= info.CLEAR_COLOR_BITS;
                        pixs[l] |= cbh;
                        pixs[l] &= info.CLEAR_FLAG_BITS;

                        flagBitsHere &= info.CLEAR_ODD_BIT;
                        flagBitsHere |= info.nonTargetOddBit;

                        pixs[l] |= flagBitsHere;
                    } else {
                        pixs[i] &= info.CLEAR_TRAV_LEFT_BIT;
                    }
                } else {
                    if((pixs[r] & info.COLOR_BITS) == 0) {
                        pixs[i] &= info.CLEAR_COLOR_BITS;
                        pixs[r] |= cbh;
                        pixs[r] &= info.CLEAR_FLAG_BITS;

                        flagBitsHere &= info.CLEAR_ODD_BIT;
                        flagBitsHere |= info.nonTargetOddBit;

                        pixs[r] |= flagBitsHere;
                    } else {
                        pixs[i] |= info.LIQUID_TRAV_LEFT_BIT;
                    }
                }
            }
        },
        /*STONE*/[](std::vector<GLubyte>& pixs, int i, PGInfo& info, GLubyte cbh, std::array<GLubyte, 16> densities){
            //STATIC, DOES NOTHING
        }
};

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

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(keyNumMap.find(key) != keyNumMap.end()) {
        SELECTED_COLOR = keyNumMap.at(key);
    }
}