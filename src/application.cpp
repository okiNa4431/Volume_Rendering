#include "application.h"

bool Application::Init()
{
	// GLFW�̏�����
	if (!glfwInit()) {
		cout << "could not init glfw" << endl;
		return false;
	}

	//�E�B���h�E�쐬
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	_window = glfwCreateWindow(1200, 900, "OpenGL Window", nullptr, nullptr);
	if (!_window)
	{
		cout << "could not create window" << endl;
		return false;
	}
	glfwMakeContextCurrent(_window);
	glClearColor(1, 1, 1, 1);

	//GLEW�̏�����
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "could not init glew" << std::endl;
		return false;
	}

	//renderer�쐬
	_renderer = renderer();
}

void Application::Run()
{
	//renderer��ݒ�
	if (!_renderer.setRenderer())
	{
		exit(EXIT_FAILURE);
	}

	//�`��Ώۂ�ݒ�
	if (!_renderer.setVolume("D:shell_512_512_271.raw"))
	{
		exit(EXIT_FAILURE);
	}

	//���[�v
	while (!glfwWindowShouldClose(_window))
	{
		//�E�B���h�E�N���A
		glClear(GL_COLOR_BUFFER_BIT);

		//�`��
		_renderer.draw();

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