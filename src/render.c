#ifndef __RENDER_UTIL

#define __RENDER_UTIL

#include "5kgl.h"

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

float ar;

void resize_callback(GLFWwindow *window, int width, int height) {
    (void) window;
    ar = (float) width / (float) height;
}

float get_aspect_ratio() {
    return ar;
}

GLFWwindow *window;
GLFWmonitor *monitor;

int w, h;
list *key_cb_list;

void __key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    for (int i = 0; i < key_cb_list->length; i++) {
        ((GLFWkeyfun) list_get(key_cb_list, i))(window, key, scancode, action, mods);
    }
}

GLFWwindow* make_window(int width, int height, char *title) {
    if (window) {
        return window;
    }

    key_cb_list = create_list();

    if (!glfwInit()) {
        fprintf(stderr, "Failed to init GLFW!\n");
        exit(EXIT_FAILURE);
    }

    monitor = NULL;

    if (width < 1 && height < 1) {
        monitor = glfwGetPrimaryMonitor();

        if (!monitor) {
            fprintf(stderr, "Failed to get monitor!\n");
            exit(EXIT_FAILURE);
        }

        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        width  = mode->width;
        height = mode->height;
    }

    glfwWindowHint(GLFW_SAMPLES, 1);

    window = glfwCreateWindow(width, height, title, monitor, NULL);

    ar = (float) width / (float) height;
    w  = width;
    h  = height;

    if (monitor) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    if (!window) {
        fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize_callback);
    glfwSetKeyCallback(window, __key_callback);

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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    set_actors(create_list());

    return window;
}

void add_key_callback(GLFWkeyfun f) {
    list_add(key_cb_list, f);
}

void remove_key_callback(GLFWkeyfun f) {
    list_remove_element(key_cb_list, f);
}

void get_cursor_position(double *x, double *y) {
    glfwGetCursorPos(window, x, y);
}

void start_main_loop() {
    glfwSetTime(0.0);
    double temp, time = 0.0;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        temp = glfwGetTime();
        step_scene(time - temp);
        time = temp;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        draw_scene();
        glfwSwapBuffers(window);
    }
    end_game();

    list_free_keep_elements(key_cb_list);

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

#endif
