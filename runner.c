#include <stdio.h>
#include <math.h>
#include "render.h"
#include "mesh.h"
#include <GLFW/glfw3.h>

GLFWwindow *window;
float yaw = 0;
float pitch = 0;

int frames;
double lasttime;

static int mvp_handle, model_handle;
static Mesh *cube;
static mat4 m, mvp;
static Camera camera;
static quat rot, q;
static Shader *shader;

void game_start() {
    shader = make_shader("color_vertex.glsl", "color_fragment.glsl");
    map_shader_handle(shader, VERT, "position");
    map_shader_handle(shader, NORM, "normal");

    mvp_handle = glGetUniformLocation(shader->prog, "mvp");
    model_handle = glGetUniformLocation(shader->prog, "model");

    cube = mesh_build_cube();
    mesh_make_vbo(cube);

    vec3 v1 = (vec3) { 3, 3, 3 };
    vec3 v2 = (vec3) { 0, 0, 0 };
    vec3 v3 = (vec3) { 0, 0, 1 };

    cam_update_view(&camera, &v1, &v2, &v3);
    cam_update_perspective(&camera, 70.0f, 1.0f, 100.0f);

    q = (quat) { 0.0, 0.0, 0.0, 1.0 };
    rot = quat_from_euler_angles(0.0, 0.0, 1.0);

    frames = 0;
    lasttime = -1;
}


void step_call(double time) {
    if (lasttime != -1) {
        if (time - lasttime > 1) {
            printf("%d frames\n", frames);
            frames = 0;
            lasttime = time;
        } else {
            frames++;
        }
    } else {
        lasttime = time;
    }

    q = quat_mult(rot, q);
    quat_to_matrix(q, m);

    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    mx -= 600;
    my -= 600;
    yaw   += mx / 100.0f;
    pitch -= my / 100.0f;

    float clamp = M_PI / 2.0f - 0.001f;

    pitch = pitch > clamp ? clamp : (pitch < -clamp ? -clamp : pitch);

    glfwSetCursorPos(window, 600, 600);

    vec3 from = (vec3) {
        5 * cos(yaw) * cos(pitch),
        5 * sin(yaw) * cos(pitch),
        5 * sin(pitch)
    };
    cam_update_view(&camera, &from, NULL, NULL);
}

void draw_call() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cam_get_mvp(mvp, &camera, m);

    bind_program_mesh(shader, cube);

    glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, mvp);
    glUniformMatrix4fv(model_handle, 1, GL_FALSE, m);

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
    window = make_window(0, 0, "5KGL");
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    set_target_framerate(0);
    
    game_start();

    start_main_loop(step_call, draw_call);
    return 0;
}
