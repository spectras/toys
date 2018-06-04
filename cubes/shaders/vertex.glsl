#version 330

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexColor;

uniform mat4 mvp;

out vec3 fragmentColor;

void main()
{
    gl_Position = mvp * vec4(vertexPos, 1);
    fragmentColor = vertexColor;
}
