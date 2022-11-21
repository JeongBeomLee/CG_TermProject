#version 330 core

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

layout (location = 0) in vec3 in_Position; //--- 위치 변수: attribute position 0

void main(void) 
{
	gl_Position = projectionTransform * viewTransform * modelTransform * vec4(in_Position, 1.0);
}