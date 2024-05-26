#version 330 core

layout(location = 0) in vec3 aPos; // ���͒��_���� (�ʒu)

uniform mat4 world;
uniform mat4 view;
uniform mat4 proj;

out vec3 texcoord;

void main(void)
{
	texcoord = aPos+0.5;
	gl_Position = proj*view*world*vec4(aPos, 1);
}