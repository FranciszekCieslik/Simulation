#version 330 core

layout(location = 0) in vec3 position;  // Object's position
layout(location = 1) in vec2 texCoord;  // Texture coordinates

uniform mat4 modelViewProjectionMatrix;

out vec2 vTexCoord;

void main() {
    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
    vTexCoord = texCoord;
}
