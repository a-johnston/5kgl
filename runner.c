#include <stdio.h>
#include "render.h"
#include "mesh.h"
#include <GLFW/glfw3.h>

static int program, pos_handle, pers_handle, view_handle;

static mesh *cube;
static GLuint vert_buff;
static GLuint tris_buff;

static mat4 pers, view;

static quat *q, *rot;

void game_start() {
    int vert = make_shader(GL_VERTEX_SHADER, "color_vertex.glsl");
    int frag = make_shader(GL_FRAGMENT_SHADER, "color_fragment.glsl");

    program = make_program(vert, frag);
    pos_handle = glGetAttribLocation(program, "position");
    pers_handle = glGetUniformLocation(program, "persp");
    view_handle = glGetUniformLocation(program, "view");

    cube = mesh_build_cube();

    float vdata[cube->verts->length * 3];
    short tdata[cube->tris->length  * 3];

    floatify_vec3(*(cube->verts), vdata);
    floatify_ivec3(*(cube->tris), tdata);

    vert_buff = make_buffer(GL_ARRAY_BUFFER, vdata, sizeof(vdata));

    tris_buff = make_buffer(GL_ARRAY_BUFFER, tdata, sizeof(tdata));

    printf("tris :  %d", cube->tris->length * 3);

    mat4_perspective(pers, 16.0f/9.0f, 90.0f, 0.1f, 10.0f);

    q = c_quat(0, 0, 0, 1);
    rot = c_quat(0, 0, 0, 1);
    *rot = quat_from_euler_angles(0.001, 0.001, 0);

    vec3 *v1 = c_vec3(-5, -5, -2);
    vec3 *v2 = c_vec3(0, 0, 0);
    vec3 *v3 = c_vec3(0, 0, 1);

    mat4_look_at(view, *v1, *v2, *v3);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void draw_call() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    *q = quat_mult(*q, *rot);

    quat_to_matrix(*q, view);

    //glUniformMatrix4fv(pers_handle, 1, GL_FALSE, pers);
    glUniformMatrix4fv(view_handle, 1, GL_FALSE, view);

    glBindBuffer(GL_ARRAY_BUFFER, vert_buff);
    glVertexAttribPointer(
        pos_handle,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(GLfloat)*3,
        (void*)0);

    glEnableVertexAttribArray(pos_handle);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tris_buff);

    glDrawElements(
        GL_TRIANGLE_STRIP,
        48,
        GL_UNSIGNED_SHORT,
        (void*)0);

    int err = glGetError();

    if (err != 0) {
        printf("Error %d\n", err);
    }
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
