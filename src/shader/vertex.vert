#version 330 core

layout(location = 0) in vec3 aPos; // ���͒��_���� (�ʒu)

void main(void)
{
	gl_Position = vec4(aPos, 1);
}