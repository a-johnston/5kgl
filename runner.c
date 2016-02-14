#include <stdio.h>
#include "render.h"
#include "mesh.h"
#include <GLFW/glfw3.h>

static int program, pos_handle;

static mesh *cube;
static GLuint vert_buff;
static GLuint tris_buff;

void game_start() {
    printf("Enter game start\n");
    int vert = make_shader(GL_VERTEX_SHADER, "color_vertex.glsl");
    int frag = make_shader(GL_FRAGMENT_SHADER, "color_fragment.glsl");

    printf("make prog %d\n", glGetError());
    program = make_program(vert, frag);
    printf("get handle %d %d\n", glGetError(), program);
    pos_handle = glGetUniformLocation(program, "position");

    printf("get cube %d\n", glGetError());
    cube = mesh_build_test();
    printf("scale cube %d\n", glGetError());
    mesh_scale(*cube, 0.5);

    printf("vdata\n");
    float vdata[cube->verts->length * 3];
    short tdata[cube->tris->length  * 3];

    printf("floatify\n");
    floatify_vec3(*(cube->verts), vdata);
    floatify_ivec3(*(cube->tris), tdata);

    vert_buff = make_buffer(GL_ARRAY_BUFFER, vdata, sizeof(vdata));

    tris_buff = make_buffer(GL_ARRAY_BUFFER, tdata, sizeof(tdata));

    printf("%d %d\n", cube->verts->length, cube->tris->length);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void draw_call() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    glBindBuffer(GL_ARRAY_BUFFER, vert_buff);
    glVertexAttribPointer(
        pos_handle,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0);

    glEnableVertexAttribArray(pos_handle);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tris_buff);

    glDrawElements(
        GL_TRIANGLE_STRIP,
        12,
        0,
        (void*)0);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main() {
    start_fullscreen(key_callback, game_start, NULL, draw_call);

    return 0;
}
