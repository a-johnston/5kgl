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

GLuint make_shader(GLenum type, const char *filename){
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

GLuint make_program(GLuint vertex_shader, GLuint fragment_shader) {
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
    ar = (float) width / (float) height;
}

void start_fullscreen(
    void (*key_callback)  (GLFWwindow*, int, int, int, int),
    void (*start_callback) (),
    void (*step_callback) (double),
    void (*draw_callback) ()
) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to init GLFW!\n");
        exit(EXIT_FAILURE);
    }

    GLFWwindow *window;
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    window = glfwCreateWindow(mode->width, mode->height, "5KGL", monitor, NULL);
    ar = (float) mode->width / (float) mode->height;

    if (!window) {
        fprintf(stderr, "Failed to grab window!\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glfwSetFramebufferSizeCallback(window, resize_callback);
    glfwSetKeyCallback(window, key_callback);

    glewInit();
    if (!GLEW_VERSION_2_0) {
        fprintf(stderr, "OpenGL 2.0 not available\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    start_callback();

    glfwSetTime(0.0);
    double time = 0.0;
    int last = 0;
    int frames = 0;
    while (!glfwWindowShouldClose(window)) {
        time = glfwGetTime();
        if ((int) time != last) {
            printf("FPS: %d\n", frames);
            frames = 0;
            last = (int) time;
        } else {
            frames++;
        }

        glfwPollEvents();
        if (step_callback) step_callback(time);
        if (draw_callback) draw_callback();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

#endif
