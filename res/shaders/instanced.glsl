#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 translation;

uniform mat4 projection;
uniform mat4 view;

void main() {
    vec3 worldPosition = translation + vec3(position, 0.0);
    gl_Position = projection * view * vec4(worldPosition, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main() {
    color = vec4(1.0, 1.0, 1.0, 1.0);
}