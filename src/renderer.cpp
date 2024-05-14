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
		exit(EXIT_FAILURE);
	}

	return true;
}

renderer::renderer()
{

}
renderer::~renderer()
{

}