#include <math.h>
#include <5kgl.h>

static Mesh *mesh;
static mat4 m, mvp;
static Camera camera;
static quat rot, q;
static Shader *shader;
static GLuint texture;

void create_call() {
    // create shader and map variables
    shader = make_shader("assets/color_vertex.glsl", "assets/color_fragment.glsl");
    texture = load_bmp("/home/adam/test.bmp");

    map_shader_attrib(shader, VERT, "position");
    map_shader_attrib(shader, NORM, "normal");

    map_shader_uniform(shader, MATRIX_4FV, "mvp", 1, &mvp);
    map_shader_uniform(shader, MATRIX_4FV, "model", 1, &m);
    map_shader_uniform(shader, TEXTURE_2D, "sample", 0, &texture);

    // load mesh and send data to gpu
    mesh = mesh_build_cube();
    mesh_make_vbo(mesh);

    // cube rotation matrix
    q = (quat) { 0.0, 0.0, 0.0, 1.0 };
    rot = quat_from_euler_angles(0.1, 0.5, 1.0);

}

void step_call(double time) {
    (void) time;
    q = quat_mult(rot, q);
    quat_to_matrix(q, m);
}

void draw_call() {
    cam_get_mvp(mvp, &camera, m);
    draw_mesh(shader, mesh);
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
    // create a fullscreen window
    make_window(-1, -1, "5kgl");
    add_key_callback(key_callback);

    // setup the camera
    vec3 from = (vec3) { 2, 2, 2 };
    vec3 to   = (vec3) { 0, 0, 0 };
    vec3 up   = (vec3) { 0, 0, 1 };
    cam_update_view(&camera, &from, &to, &up);
    cam_update_perspective(&camera, 70.0f, 1.0f, 100.0f);

    //set the scene
    Actor *monkey = make_actor(create_call, step_call, draw_call);
    add_actor(monkey);

    start_main_loop();

    return EXIT_SUCCESS;
}
