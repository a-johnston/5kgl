uniform sampler2D sample;

varying vec3 color;
varying vec3 norm;

void main()
{
    float intensity = clamp(dot(normalize(norm), normalize(vec3(1, 1, 1))), 0.0, 1.0);
    vec3 shaded_color = color * intensity;
    gl_FragColor = vec4(shaded_color, 1.0) + vec4(.1, .1, .1, 0.0);
}
