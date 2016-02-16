attribute vec3 position;

uniform mat4 persp;
uniform mat4 view;

varying vec3 color;

void main()
{
	color = position + vec3(0.5, 0.5, 0.5);
    gl_Position = view * vec4(position, 1.0);
}
