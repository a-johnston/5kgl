#include <5kgl.h>

struct demo_data {
    Mesh *mesh;
    Shader *shader;
    list *uniforms;

    mat4 m, mvp;
    Camera camera;
    quat rot, q;
};

void* create_call() {
    struct demo_data *data = (struct demo_data*) malloc(sizeof(struct demo_data));

    // create shader and map variables
    data->shader = make_shader("assets/color_vertex.glsl", "assets/color_fragment.glsl");

    data->uniforms = create_list();

    map_shader_attrib(data->shader, VERT, "position");
    map_shader_attrib(data->shader, NORM, "normal");

    map_shader_uniform(data->shader, MATRIX_4FV, "mvp", 1);
    map_shader_uniform(data->shader, MATRIX_4FV, "model", 1);

    list_add(data->uniforms, &data->mvp);
    list_add(data->uniforms, &data->m);

    // load mesh and send data to gpu
    // data->mesh = mesh_build_cube();
    data->mesh = read_obj("assets/untitled.obj");

    mesh_make_vbo(data->mesh);

    // cube rotation matrix
    data->q = (quat) { 0.0, 0.0, 0.0, 1.0 };
    data->rot = quat_from_euler_angles(0.1, 0.5, 1.0);

    // setup the camera
    vec3 from = (vec3) { 2, 2, 2 };
    vec3 to   = (vec3) { 0, 0, 0 };
    vec3 up   = (vec3) { 0, 0, 1 };
    cam_update_view(&data->camera, &from, &to, &up);
    cam_update_perspective(&data->camera, 70.0f, 1.0f, 100.0f);

    return data;
}

void step_call(void *void_data, double time) {
    (void) time;
    struct demo_data *data = (struct demo_data*) void_data;
    data->q = quat_mult(data->rot, data->q);
    quat_to_matrix(data->q, data->m);
}

void draw_call(void *void_data) {
    struct demo_data *data = (struct demo_data*) void_data;
    cam_get_mvp(data->mvp, &data->camera, data->m);
    draw_mesh(data->shader, data->mesh, data->uniforms);
    check_gl_error();
}

void destroy_call(void *void_data) {
    struct demo_data *data = (struct demo_data*) void_data;
    free_mesh(data->mesh);
    free_shader(data->shader);
    list_free_keep_elements(data->uniforms);
    free(data);
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

    //set the scene
    Actor actor = make_actor(create_call, step_call, draw_call, destroy_call);
    add_actor(&actor);

    start_main_loop();

    return EXIT_SUCCESS;
}
