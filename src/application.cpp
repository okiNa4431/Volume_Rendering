#include "application.h"

bool Application::Init()
{
	// GLFW�̏�����
	if (!glfwInit()) {
		return false;
	}

	//�E�B���h�E�쐬
	window = glfwCreateWindow(1200, 900, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		return false;
	}
}

void Application::Run()
{
	//�E�B���h�E��ݒ�
	glfwMakeContextCurrent(window);

	//���[�v
	while (!glfwWindowShouldClose(window))
	{
		//�E�B���h�E�N���A
		glClear(GL_COLOR_BUFFER_BIT);

		//�X���b�v
		glfwSwapBuffers(window);

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
	window = nullptr;
}
Application::~Application()
{

}