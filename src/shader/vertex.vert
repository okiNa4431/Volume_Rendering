#version 330 core

layout(location = 0) in vec3 aPos; // 入力頂点属性 (位置)

void main(void)
{
	gl_Position = vec4(aPos, 1);
}