#version 330 core

layout(location = 0) in vec3 aPos; // “ü—Í’¸“_‘®« (ˆÊ’u)

void main(void)
{
	gl_Position = vec4(aPos, 1);
}