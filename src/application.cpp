#include "application.h"

bool Application::Init()
{
	// GLFWの初期化
	if (!glfwInit()) {
		return false;
	}

	//ウィンドウ作成
	_window = glfwCreateWindow(1200, 900, "OpenGL Window", NULL, NULL);
	if (!_window)
	{
		return false;
	}

	//renderer作成
	_renderer = renderer();
}

void Application::Run()
{
	//ウィンドウを設定
	glfwMakeContextCurrent(_window);
	glClearColor(1, 1, 1, 1);

	//rendererを設定
	if (!_renderer.setRenderer())
	{
		exit(EXIT_FAILURE);
	}

	//ループ
	while (!glfwWindowShouldClose(_window))
	{
		//ウィンドウクリア
		glClear(GL_COLOR_BUFFER_BIT);

		//スワップ
		glfwSwapBuffers(_window);

		//イベントのポーリング
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