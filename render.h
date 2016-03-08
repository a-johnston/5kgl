#ifndef __RENDER_UTIL

#define __RENDER_UTIL

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "util.h"

void print_gl_log(
    GLuint object,
    PFNGLGETSHADERIVPROC glGet__iv,
    PFNGLGETSHADERINFOLOGPROC glGet__InfoLog
) {
    GLint log_length;

    char *log;
    glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
    log = (char*) malloc(log_length);
    glGet__InfoLog(object, log_length, NULL, log);
    fprintf(stderr, "%s", log);
    free(log);
}

void check_gl_error() {
    int err = glGetError();

    if (err != 0) {
        fprintf(stderr, "Error %d\n", err);
    }
}

GLuint _make_shader(GLenum type, const char *filename){
    GLint length;
    GLchar *source = (GLchar*) read_file(filename, &length);
    GLuint shader;
    GLint shader_ok;

    if (!source)
        return 0;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&source, &length);

    free(source);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok) {
        fprintf(stderr, "Failed to compile %s:\n", filename);
        print_gl_log(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint _make_program(GLuint vertex_shader, GLuint fragment_shader) {
    GLint program_ok;

    GLuint program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &program_ok);

    if (!program_ok) {
        fprintf(stderr, "Failed to link shader program:\n");
        print_gl_log(program, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(program);
        return 0;
    }

    return program;
}

static float ar = 0.0f;

void resize_callback(GLFWwindow *window, int width, int height) {
    (void) window;
    ar = (float) width / (float) height;
}

float get_aspect_ratio() {
    return ar;
}

GLFWwindow *window;

GLFWwindow* make_window(int width, int height, char *title) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to init GLFW!\n");
        exit(EXIT_FAILURE);
    }

    GLFWmonitor *monitor = NULL;

    if (width < 1 && height < 1) {
        monitor = glfwGetPrimaryMonitor();

        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        width  = mode->width;
        height = mode->height;
    }

    window = glfwCreateWindow(width, height, title, monitor, NULL);
    ar = (float) width / (float) height;

    if (!window) {
        fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize_callback);

    GLenum err;
    if ((err = glewInit()) != GLEW_OK) {
        fprintf(stderr, "Failed to init glew: %s\n", glewGetErrorString(err));
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    if (!GLEW_VERSION_3_0) {
        fprintf(stderr, "OpenGL 3.0 not available\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return window;
}

void set_target_framerate(int fps) {
    if (fps > 0) {
        glfwSwapInterval(60.0f / fps);
    } else {
        glfwSwapInterval(0);
    }
}

void start_main_loop(
    void (*step_callback) (double),
    void (*draw_callback) ()
) {
    glfwSetTime(0.0);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        if (step_callback) step_callback(glfwGetTime());
        if (draw_callback) draw_callback();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

#endif
