#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cstdlib>
#include <array>


#define TEXT_LOADER_IMPLEMENTATION
#define JPGFUNCS_IMPLEMENTATION
#define JPG_CALLBACK_IMPLEMENTATION
#include "jpgfuncs.h"
#include "jpgglobals.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


//Uncomment this stuff to remove console when done:
//#include <Windows.h>
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
int main() {
    std::cout << "Display width: " << DWIDTH << '\n' 
              << "Display height: " << DHEIGHT << '\n';
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    if (!(WINDOW = glfwCreateWindow(WWIDTH, WHEIGHT, "Jackson's Powder Game", NULL, NULL))) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Load the icon image
    int width, height, channels;
    unsigned char* iconData = stbi_load("assets/pngicon.png", &width, &height, &channels, 0);
    if (!iconData) {
        std::cerr << "Failed to load icon: " << stbi_failure_reason() << std::endl;
    }
    if (iconData) {
        GLFWimage icon;
        icon.width = width;
        icon.height = height;
        icon.pixels = iconData;

        glfwSetWindowIcon(WINDOW, 1, &icon);
        stbi_image_free(iconData);
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, DWIDTH, DHEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //The quad that the entire screen is drawn on
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

    glUseProgram(SHADER_PROG1);
    glUniform3fv(glGetUniformLocation(SHADER_PROG1, "colorPalette"), 16, glm::value_ptr(colorPalette[0]));

    while(!glfwWindowShouldClose(WINDOW)) {

        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if(MOUSE_CLICKED) {
            jpg::draw_at_cursor(MOUSEX, MOUSEY, PIXELS, WHEIGHT, RATIO, DWIDTH);
        }

        //jpg::randomize_noise_test();
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, DWIDTH, DHEIGHT, GL_RED, GL_UNSIGNED_BYTE, PIXELS.data());

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(WINDOW);
        glfwPollEvents();
        jpg::update_time(DELTA_TIME, LAST_FRAME);
    }

    glfwDestroyWindow(WINDOW);
    glfwTerminate();

    return EXIT_SUCCESS;
}