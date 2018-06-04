#version 330

/** Vertex shader runs first. It processes one vertex.
 * Its main purpose is tranforming coordinates and sending relevant
 * data to the next shaders in the pipeline.
 *
 * This is a minimal vertex shader.
 */

layout(location = 0) in vec3 vertexPos;     // Coordinates
layout(location = 1) in vec3 vertexColor;   // RGB color - this shader doesn't support alpha

uniform mat4 mvp;                           // Transformation matrix

out vec3 fragmentColor;                     // This gets sent to the fragment shader

void main()
{
    gl_Position = mvp * vec4(vertexPos, 1); // Apply transformation matrix
    fragmentColor = vertexColor;            // Forward vertex color data
}
