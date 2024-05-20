#include "application.h"

bool Application::Init()
{
	// GLFWの初期化
	if (!glfwInit()) {
		cout << "could not init glfw" << endl;
		return false;
	}

	//ウィンドウ作成
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

	//GLEWの初期化
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "could not init glew" << std::endl;
		return false;
	}

	//renderer作成
	_renderer = renderer();
}

void Application::Run()
{
	//rendererを設定
	if (!_renderer.setRenderer())
	{
		exit(EXIT_FAILURE);
	}

	//描画対象を設定
	if (!_renderer.setVolume("D:shell_512_512_271.raw"))
	{
		exit(EXIT_FAILURE);
	}

	//ループ
	while (!glfwWindowShouldClose(_window))
	{
		//ウィンドウクリア
		glClear(GL_COLOR_BUFFER_BIT);

		//描画
		_renderer.draw();

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