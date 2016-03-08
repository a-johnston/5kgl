#include <stdio.h>
#include "render.h"
#include "mesh.h"
#include <GLFW/glfw3.h>

static int program, pos_handle, mvp_handle;

static mesh *cube;

static GLuint vert_buff;
static GLuint tris_buff;

static mat4 pers, view, vp, m, mvp;

static quat rot, q;

void game_start() {
    int vert = make_shader(GL_VERTEX_SHADER, "color_vertex.glsl");
    int frag = make_shader(GL_FRAGMENT_SHADER, "color_fragment.glsl");

    program = make_program(vert, frag);
    pos_handle = glGetAttribLocation(program, "position");
    mvp_handle = glGetUniformLocation(program, "mvp");

    cube = mesh_build_cube();

    vert_buff = make_vert_buffer(cube);
    tris_buff = make_tri_buffer(cube);

    mat4_perspective(pers, 70.0f, 1.0f, 100.0f);

    vec3 v1 = (vec3) { 3, 3, 3 };
    vec3 v2 = (vec3) { 0, 0, 0 };
    vec3 v3 = (vec3) { 0, 0, 1 };

    mat4_look_at(view, v1, v2, v3);

    mat4_mult(vp, view, pers);

    q = (quat) { 0.0, 0.0, 0.0, 1.0 };
    rot = quat_from_euler_angles(0.0, 0.0, 1.0);
}

void step_call(double time) {
    (void) time;
    q = quat_mult(rot, q);
    quat_to_matrix(q, m);
}

void draw_call() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    mat4_mult(mvp, m, vp);

    glBindBuffer(GL_ARRAY_BUFFER, vert_buff);
    glVertexAttribPointer(pos_handle, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(pos_handle);

    glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, mvp);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tris_buff);
    glDrawElements(GL_TRIANGLES, cube->attr[TRIS]->length * 3, GL_UNSIGNED_SHORT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    int err = glGetError();

    if (err != 0) {
        printf("Error %d\n", err);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void) scancode;
    (void) mods;
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main() {
    GLFWwindow *window = make_window(600, 400, "5KGL");
    glfwSetKeyCallback(window, key_callback);
    
    game_start();

    start_main_loop(step_call, draw_call);
    return 0;
}
