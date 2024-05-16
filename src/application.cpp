#include "application.h"

bool Application::Init()
{
	// GLFW�̏�����
	if (!glfwInit()) {
		printf("could not init glfw\n");
		return false;
	}

	//�E�B���h�E�쐬
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	_window = glfwCreateWindow(1200, 900, "OpenGL Window", nullptr, nullptr);
	if (!_window)
	{
		printf("could not create window\n");
		return false;
	}

	//renderer�쐬
	_renderer = renderer();
}

void Application::Run()
{
	//�E�B���h�E��ݒ�
	glfwMakeContextCurrent(_window);
	glClearColor(1, 1, 1, 1);

	//renderer��ݒ�
	if (!_renderer.setRenderer())
	{
		exit(EXIT_FAILURE);
	}

	//���[�v
	while (!glfwWindowShouldClose(_window))
	{
		//�E�B���h�E�N���A
		glClear(GL_COLOR_BUFFER_BIT);

		//�X���b�v
		glfwSwapBuffers(_window);

		//�C�x���g�̃|�[�����O
		glfwPollEvents();
	}
}

void Application::Terminate()
{
	glfwTerminate();
}

Application& Application::Instance()
{
	static Application instance;
	return instance;
}
Application::Application()
{
	_window = nullptr;
}
Application::~Application()
{

}