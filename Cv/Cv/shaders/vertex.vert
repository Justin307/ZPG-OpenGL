#version 330

layout(location=0) in vec3 vp;
layout(location=1) in vec3 vn;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 color;
out vec4 worldPos;
out vec3 worldNorm;

void main () 
{
     worldPos = modelMatrix * vec4(vp, 1.0);
     mat4 normal = modelMatrix;
     worldNorm = vec3(normal * vec4(vn, 1.0));
     gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4 (vp, 1.0);
}