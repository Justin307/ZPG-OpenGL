#version 330

layout(location=0) in vec3 vp;
layout(location=1) in vec3 vn;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 color;
out vec4 worldPos;
out vec3 worldNorm;
out vec3 cameraPos;

void main () 
{
    color = vn;
    cameraPos = vec3(inverse(viewMatrix)[3]);
    worldPos = modelMatrix * vec4(vp, 1.0);
    mat3 normal = transpose(inverse(mat3(modelMatrix)));
    worldNorm = normal * vn;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4 (vp, 1.0);
}