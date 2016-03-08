#include <stdio.h>
#include "render.h"
#include "mesh.h"
#include <GLFW/glfw3.h>

static int mvp_handle;

static Mesh *cube;

static mat4 m, mvp;

static Camera camera;

static quat rot, q;

static Shader *shader;

void game_start() {
    shader = make_shader("color_vertex.glsl", "color_fragment.glsl");
    map_shader_handle(shader, VERT, "position");
    mvp_handle = glGetUniformLocation(shader->prog, "mvp");

    cube = mesh_build_cube();
    mesh_make_vbo(cube);

    vec3 v1 = (vec3) { 3, 3, 3 };
    vec3 v2 = (vec3) { 0, 0, 0 };
    vec3 v3 = (vec3) { 0, 0, 1 };

    cam_update_view(&camera, &v1, &v2, &v3);
    cam_update_perspective(&camera, 70.0f, 1.0f, 100.0f);

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

    cam_get_mvp(mvp, &camera, &m);

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
