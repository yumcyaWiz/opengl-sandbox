#version 460 core

layout(location = 0) out float fragmentDepth;

void main() {
    fragmentDepth = gl_FragCoord.z;
}