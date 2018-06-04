#version 330

/* Fragment shader runs after rasterization. It processes one fragment.
 * Its main purpose is defining the color(s) and material.
 *
 * This is a minimal fragment shader.
 */

in vec3 fragmentColor;              // Color passed by vertex shader
out vec3 color;                     // Where to write color data into

void main()
{
    color = fragmentColor;          // Simply copy the color
}
