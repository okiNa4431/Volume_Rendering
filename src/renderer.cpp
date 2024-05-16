#include "renderer.h"
#include "io.h"

bool renderer::setShaderFile()
{
	string vertexShaderSource = read_shader("vertex.vert");
	string fragmentShaderSource = read_shader("fragment.frag");

	return true;
}

bool renderer::setRenderer()
{
	if (!setShaderFile())
	{
		printf("could not set shader file\n");
		return false;
	}

	return true;
}

renderer::renderer()
{

}
renderer::~renderer()
{

}