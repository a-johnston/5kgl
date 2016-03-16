#include <math.h>
#include <5kgl.h>
#include <GLFW/glfw3.h>

GLFWwindow *window;
float yaw = 0;
float pitch = 0;

static Mesh *cube;
static mat4 m, mvp;
static Camera camera;
static quat rot, q;
static Shader *shader;

void step_call(double time) {
    (void) time;

    //update cube animation
    q = quat_mult(rot, q);
    quat_to_matrix(q, m);

    //move camera around
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
    cam_get_mvp(mvp, &camera, m);
    draw_mesh(shader, cube);
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

    //create shader and map variables
    shader = make_shader("color_vertex.glsl", "color_fragment.glsl");

    map_shader_attrib(shader, VERT, "position");
    map_shader_attrib(shader, NORM, "normal");

    map_shader_uniform(shader, MATRIX_4FV, "mvp", 1, &mvp);
    map_shader_uniform(shader, MATRIX_4FV, "model", 1, &m);

    //make cube mesh and send data to gpu
    cube = mesh_build_cube();
    mesh_make_vbo(cube);

    //setup the camera
    vec3 up = (vec3) { 0, 0, 1 };
    cam_update_view(&camera, NULL, NULL, &up);
    cam_update_perspective(&camera, 70.0f, 1.0f, 100.0f);

    //cube rotation matrix
    q = (quat) { 0.0, 0.0, 0.0, 1.0 };
    rot = quat_from_euler_angles(0.0, 0.0, 1.0);

    start_main_loop(step_call, draw_call);
    return 0;
}
