attribute vec3 position;

uniform mat4 mvp;

varying vec3 color;

void main()
{
	color = position * 0.4 + vec3(0.5, 0.5, 0.5);
    gl_Position = mvp * vec4(position, 1.0);
}
