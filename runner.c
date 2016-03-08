#include <stdio.h>
#include "render.h"
#include "mesh.h"
#include <GLFW/glfw3.h>

static int mvp_handle;

static Mesh *cube;

static mat4 pers, view, vp, m, mvp;

static quat rot, q;

Shader *shader;

void game_start() {
    shader = make_shader("color_vertex.glsl", "color_fragment.glsl");
    map_shader_handle(shader, VERT, "position");
    mvp_handle = glGetUniformLocation(shader->prog, "mvp");

    cube = mesh_build_cube();
    mesh_make_vbo(cube);

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

    mat4_mult(mvp, m, vp);

    bind_program_mesh(shader, cube);

    glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, mvp);

    draw_mesh(cube);
    unbind_program_mesh(shader, cube);
    check_gl_error();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void) scancode;
    (void) mods;
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main() {
    GLFWwindow *window = make_window(0, 0, "5KGL");
    glfwSetKeyCallback(window, key_callback);
    
    game_start();

    start_main_loop(step_call, draw_call);
    return 0;
}
