#include <math.h>
#include <5kgl.h>

static Mesh *mesh;
static mat4 m, mvp;
static Camera camera;
static quat rot, q;
static Shader *shader;

list *uniforms;

void* create_call() {
    // create shader and map variables
    shader = make_shader("assets/color_vertex.glsl", "assets/color_fragment.glsl");

    uniforms = create_list();

    map_shader_attrib(shader, VERT, "position");
    map_shader_attrib(shader, NORM, "normal");

    map_shader_uniform(shader, MATRIX_4FV, "mvp", 1);
    map_shader_uniform(shader, MATRIX_4FV, "model", 1);

    list_add(uniforms, &mvp);
    list_add(uniforms, &m);

    // load mesh and send data to gpu
    mesh = mesh_build_cube();
    mesh_make_vbo(mesh);

    // cube rotation matrix
    q = (quat) { 0.0, 0.0, 0.0, 1.0 };
    rot = quat_from_euler_angles(0.1, 0.5, 1.0);

    return NULL;
}

void step_call(void *data, double time) {
    (void) time;
    (void) data;
    q = quat_mult(rot, q);
    quat_to_matrix(q, m);
}

void draw_call(void *data) {
    (void) data;
    cam_get_mvp(mvp, &camera, m);
    draw_mesh(shader, mesh, uniforms);
    check_gl_error();
}

void destroy_call(void *data) {
    (void) data;
    free_mesh(mesh);
    free_shader(shader);
    list_free_keep_elements(uniforms);
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
    Actor *actor = make_actor(create_call, step_call, draw_call, destroy_call);
    add_actor(actor);

    start_main_loop();

    return EXIT_SUCCESS;
}
