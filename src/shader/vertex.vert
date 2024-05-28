#version 330 core

layout(location = 0) in vec3 aPos; // ���͒��_���� (�ʒu)

uniform mat4 MVP;

void main(void)
{
	gl_Position = MVP*vec4(aPos, 1);
}