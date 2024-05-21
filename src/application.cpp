#include "application.h"

float scrool_yoffset = 0.0;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	std::cout << "マウスの位置 - X: " << xpos << ", Y: " << ypos << std::endl;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			std::cout << "左ボタンが押されました" << std::endl;
		}
		else if (action == GLFW_RELEASE)
		{
			std::cout << "左ボタンが放されました" << std::endl;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			std::cout << "右ボタンが押されました" << std::endl;
		}
		else if (action == GLFW_RELEASE)
		{
			std::cout << "右ボタンが放されました" << std::endl;
		}
	}
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	std::cout << "スクロール - Xオフセット: " << xoffset << ", Yオフセット: " << yoffset << std::endl;
	scrool_yoffset = yoffset;
}

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
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

	//αブレンディング
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	_window = glfwCreateWindow(512, 512, "Volume Rendering", nullptr, nullptr);
	if (!_window)
	{
		cout << "could not create window" << endl;
		return false;
	}
	glfwMakeContextCurrent(_window);
	glClearColor(0, 0, 0, 1);

	//マウス入力
	glfwSetCursorPosCallback(_window, mouse_callback);
	glfwSetMouseButtonCallback(_window, mouse_button_callback);
	glfwSetScrollCallback(_window, scroll_callback);

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
		_renderer.setWorldParams(scrool_yoffset);
		_renderer.draw();

		//スワップ
		glfwSwapBuffers(_window);

		//イベントのポーリング
		glfwPollEvents();
	}
}

void Application::Terminate()
{
	_renderer.terminate();
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