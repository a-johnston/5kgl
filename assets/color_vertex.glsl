attribute vec3 position;
attribute vec3 normal;

uniform mat4 mvp;
uniform mat4 model;

varying vec3 color;
varying vec3 norm;

void main()
{
    norm = vec3(model * vec4(normal, 1.0));
    color = position * 0.4 + vec3(0.5, 0.5, 0.5);
    gl_Position = mvp * vec4(position, 1.0);
}
