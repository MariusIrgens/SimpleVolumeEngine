#version 460

in vec2 position;
out vec2 fragPosition;

out vec2 pp;

void main()
{
    // The vertex shader basically just pass positions through to the fragment shader.
    // Each pixel in the quad then calculates its position using interpolation.
    fragPosition = position;
    gl_Position = vec4(position, 0.0, 1.0);
}