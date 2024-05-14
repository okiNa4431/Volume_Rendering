#include "application.h"

bool Application::Init()
{
	// GLFWの初期化
	if (!glfwInit()) {
		return false;
	}

	//ウィンドウ作成
	window = glfwCreateWindow(1200, 900, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		return false;
	}
}

void Application::Run()
{
	//ウィンドウを設定
	glfwMakeContextCurrent(window);

	//ループ
	while (!glfwWindowShouldClose(window))
	{
		//ウィンドウクリア
		glClear(GL_COLOR_BUFFER_BIT);

		//スワップ
		glfwSwapBuffers(window);

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
	window = nullptr;
}
Application::~Application()
{

}