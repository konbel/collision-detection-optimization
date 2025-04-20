#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 translation;
layout(location = 2) in vec3 color;

uniform mat4 projection;
uniform mat4 view;

out vec4 vColor;

void main() {
    vec2 worldPosition = translation + position;
    gl_Position = projection * view * vec4(worldPosition, 0.0, 1.0);

    vColor = vec4(color, 1.0);
}

#shader fragment
#version 330 core

in vec4 vColor;
out vec4 color;

void main() {
    color = vColor;
}