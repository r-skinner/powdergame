#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cstdlib>

#define TEXT_LOADER_IMPLEMENTATION
#include "textloader.h"

#define PERLIN_IMP
#include "perlin.h"

GLFWwindow* WINDOW;
int WWIDTH = 1080;
int WHEIGHT = 760;

GLuint SHADER_PROG1;
GLuint TEXTURE_ID;

//TIME
double DELTA_TIME = 0;
double LAST_FRAME = 0;

//MOUSE
bool MOUSE_CLICKED = false;

std::vector<GLubyte> PIXELS(WWIDTH * WHEIGHT * 3);

Perlin p;

void update_time() {
    double current_frame = glfwGetTime();
    DELTA_TIME = current_frame - LAST_FRAME;
    LAST_FRAME = current_frame;
}


void randomize_noise_test() {
    static double currx = 0.0;
    static double curry = 0.0;
    static double currz = 0.0;

    currx += DELTA_TIME;
    curry += DELTA_TIME;
    currz += DELTA_TIME;
    // for(GLubyte &a : PIXELS) {
    //     a = static_cast<GLubyte>(p.noise());
    // }
    for(int x = 0; x < WHEIGHT; x++) {
        for(int i = 0; i < WWIDTH*3; i+=3) {
            int index = x * WWIDTH*3 + i;
            PIXELS[index] = static_cast<GLubyte>(
                std::min(
                    std::max(
                        p.noise(currx + (0.03 * i), curry + (0.03 * x), currz)*255, 0.0), 254.0));
            PIXELS[index+1] = static_cast<GLubyte>(
                std::min(
                    std::max(
                        p.noise(currx + (0.03 * i), curry + (0.03 * x), currz)*255, 0.0), 254.0));
            PIXELS[index+2] = static_cast<GLubyte>(
                std::min(
                    std::max(
                        p.noise(currx + (0.03 * i), curry + (0.03 * x), currz)*255, 0.0), 254.0));
        }
    }
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
        int index = (yp * (WWIDTH*3)) + (xpos * 3);
        PIXELS[index] = 254;
        PIXELS[index+1] = 254;
        PIXELS[index+2] = 254;
    }
}

int main() {
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WWIDTH, WHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

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
        glBindTexture(GL_TEXTURE_2D, TEXTURE_ID);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WWIDTH, WHEIGHT, GL_RGB, GL_UNSIGNED_BYTE, PIXELS.data());

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(WINDOW);
        glfwPollEvents();
        update_time();
    }

    glfwDestroyWindow(WINDOW);
    glfwTerminate();

    return EXIT_SUCCESS;
}