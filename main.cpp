#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cstdlib>

#define TEXT_LOADER_IMPLEMENTATION
#include "textloader.h"

GLFWwindow* WINDOW;

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

std::vector<GLubyte> PIXELS(DWIDTH * DHEIGHT * 3);

void randomize_noise_test() {
    for(GLubyte &a : PIXELS) {
        a = static_cast<GLubyte>(rand() % 254);
    }
}

void update_time() {
    double current_frame = glfwGetTime();
    DELTA_TIME = current_frame - LAST_FRAME;
    LAST_FRAME = current_frame;
}

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
    if(MOUSE_CLICKED) {
        int yp = WHEIGHT - ypos;

        int dx, dy;
        dx = static_cast<int>(xpos*RATIO);
        dy = static_cast<int>(yp*RATIO);

        int index = (dy * (DWIDTH*3)) + (dx * 3);
        PIXELS[index] = 254;
        PIXELS[index+1] = 254;
        PIXELS[index+2] = 254;
    }
}

int main() {
    std::cout << "Display width: " << DWIDTH << '\n' 
              << "Display height: " << DHEIGHT << '\n';
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    if (!(WINDOW = glfwCreateWindow(WWIDTH, WHEIGHT, "Powder Game-o", NULL, NULL))) {
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(WINDOW);
    glewInit();

    glfwSetMouseButtonCallback(WINDOW, mouse_button_callback);
    glfwSetCursorPosCallback(WINDOW, cursor_position_callback);

    std::string vertexShaderSrc;
    std::string fragmentShaderSrc;
    load_text("shad/vert.glsl", vertexShaderSrc);
    load_text("shad/frag.glsl", fragmentShaderSrc);
    const GLchar *vertexGLChars = vertexShaderSrc.c_str();
    const GLchar *fragGLChars = fragmentShaderSrc.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexGLChars, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragGLChars, NULL);
    glCompileShader(fragmentShader);

    SHADER_PROG1 = glCreateProgram();
    glAttachShader(SHADER_PROG1, vertexShader);
    glAttachShader(SHADER_PROG1, fragmentShader);
    glLinkProgram(SHADER_PROG1);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenTextures(1, &TEXTURE_ID);
    glBindTexture(GL_TEXTURE_2D, TEXTURE_ID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DWIDTH, DHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    float quadVertices[] = {
        // positions   // texture coords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // position attribute
    GLint pos_attrib = glGetAttribLocation(SHADER_PROG1, "pos");
    glEnableVertexAttribArray(pos_attrib);
    glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // texture coord attribute
    GLint tex_attrib = glGetAttribLocation(SHADER_PROG1, "texcoord");
    glEnableVertexAttribArray(tex_attrib);
    glVertexAttribPointer(tex_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    while(!glfwWindowShouldClose(WINDOW)) {

        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(SHADER_PROG1);
        randomize_noise_test();
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, DWIDTH, DHEIGHT, GL_RGB, GL_UNSIGNED_BYTE, PIXELS.data());

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(WINDOW);
        glfwPollEvents();
        update_time();
    }

    glfwDestroyWindow(WINDOW);
    glfwTerminate();

    return EXIT_SUCCESS;
}