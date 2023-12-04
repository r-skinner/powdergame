#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "main.h"
#include "config.h"
#include "utils.h"
#include "jpgfuncs.h"
#include "jpgglobals.h"

using namespace std;

double updateSimTimer = 0.0;

int main() {
    printf("Display width: %d\n", WINDOW_WIDTH);
    printf("Display height: %d\n", WINDOW_HEIGHT);

    GLFWwindow *window;
    std::vector<GLubyte> PIXELS(DISPLAY_WIDTH * DISPLAY_HEIGHT);

    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    if (!(window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Jackson's Powder Game", nullptr, nullptr))) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    load_icon(window, "assets/pngicon.png");

    glfwMakeContextCurrent(window);
    glewInit();

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);

    // todo load shader
    string vertexShaderSrc;
    string fragmentShaderSrc;
    load_text("assets/shaders/vert.glsl", vertexShaderSrc);
    load_text("assets/shaders/frag.glsl", fragmentShaderSrc);
    const GLchar *vertexGLChars = vertexShaderSrc.c_str();
    const GLchar *fragGLChars = fragmentShaderSrc.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexGLChars, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragGLChars, nullptr);
    glCompileShader(fragmentShader);
    //todo end load shader

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertexShader);
    glAttachShader(shader_program, fragmentShader);
    glLinkProgram(shader_program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint TEXTURE_ID;
    glGenTextures(1, &TEXTURE_ID);
    glBindTexture(GL_TEXTURE_2D, TEXTURE_ID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, QUAD_VERTS_LEN, QUAD_VERTS, GL_STATIC_DRAW);

    // position attribute
    GLint pos_attrib = glGetAttribLocation(shader_program, "pos");
    glEnableVertexAttribArray(pos_attrib);
    glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) nullptr);

    // texture coord attribute
    GLint tex_attrib = glGetAttribLocation(shader_program, "texcoord");
    glEnableVertexAttribArray(tex_attrib);
    glVertexAttribPointer(tex_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));

    glUseProgram(shader_program);
    glUniform3fv(glGetUniformLocation(shader_program, "colorPalette"), 16, glm::value_ptr(colorPalette[0]));

    while (!glfwWindowShouldClose(window)) {
        update(window, PIXELS);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

void update(GLFWwindow *window, vector<GLubyte> &PIXELS) {

    bool isDrawNeeded = false;
    if (mouse.left_click) {
        drawAtCursor(PIXELS, SELECTED_COLOR);
        isDrawNeeded = true;
    }

    // set top corner
    PIXELS[DISPLAY_WIDTH * (DISPLAY_HEIGHT - 1)] = SELECTED_COLOR;

    if (updateSimTimer > UPDATE_INTERVAL) {
        updateSimTimer = 0.0;
        updateSimulations(PIXELS, ODD_FRAME);
        isDrawNeeded = true;
    } else {
        updateSimTimer += DELTA_TIME;
    }
    if (isDrawNeeded) draw(window, PIXELS);
    updateTime(DELTA_TIME, LAST_FRAME);
}

void draw(GLFWwindow *window, vector<GLubyte> &PIXELS) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, GL_RED, GL_UNSIGNED_BYTE, PIXELS.data());
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwSwapBuffers(window);
    glfwPollEvents();
}
