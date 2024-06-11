#include "application.h"

//�}�E�X�E�L�[�{�[�h�̓��͒l
float scrool_yoffset = 0.0;
float nowMouseX = 0.0f;
float nowMouseY = 0.0f;
float lastMouseX = 0.0f;
float lastMouseY = 0.0f;
bool translate_flag = false;
bool rotate_flag = false;
bool right_key_down = false;
bool left_key_down = false;
bool up_key_down = false;
bool down_key_down = false;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	lastMouseX = nowMouseX;
	lastMouseY = nowMouseY;
	nowMouseX = xpos;
	nowMouseY = ypos;
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			rotate_flag = true;
		}
		else if (action == GLFW_RELEASE)
		{
			rotate_flag = false;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		if (action == GLFW_PRESS)
		{
			translate_flag = true;
		}
		else if (action == GLFW_RELEASE)
		{
			translate_flag = false;
		}
	}
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	scrool_yoffset = yoffset;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_RIGHT) {
			right_key_down = true;
		}
		else if (key == GLFW_KEY_LEFT) {
			left_key_down = true;
		}
		if (key == GLFW_KEY_UP)
		{
			up_key_down = true;
		}
		else if (key == GLFW_KEY_DOWN)
		{
			down_key_down = true;
		}
	}
}

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
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

	//���u�����f�B���O
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	_window = glfwCreateWindow(width, height, "Volume Rendering", nullptr, nullptr);
	if (!_window)
	{
		cout << "could not create window" << endl;
		return false;
	}
	glfwMakeContextCurrent(_window);
	glClearColor(0, 0, 0, 1);

	//�}�E�X�E�L�[�{�[�h����
	glfwSetCursorPosCallback(_window, mouse_callback);
	glfwSetMouseButtonCallback(_window, mouse_button_callback);
	glfwSetScrollCallback(_window, scroll_callback);
	glfwSetKeyCallback(_window, key_callback);

	//GLEW�̏�����
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cout << "could not init glew" << endl;
		return false;
	}

	//renderer�쐬
	_renderer = renderer(make_pair(width,height));
}

void Application::Run()
{
	//renderer��ݒ�
	if (!_renderer.setRenderer())
	{
		exit(EXIT_FAILURE);
	}
	//�`��Ώۂ�ݒ�
		//����
	string path = "D:Volume/Nio-uint16-692x349x241-1.5mm_max.raw";
	int voxelsSize[3] = { 692, 349, 241 };
	if (!_renderer.setVolume(path, voxelsSize))
	{
		exit(EXIT_FAILURE);
	}

	//���[�v
	while (!glfwWindowShouldClose(_window))
	{
		//�E�B���h�E�N���A
		glClear(GL_COLOR_BUFFER_BIT);

		//�`��
		_renderer.setWorldParams(scrool_yoffset, lastMouseX, nowMouseX, lastMouseY, nowMouseY, rotate_flag, translate_flag, right_key_down, left_key_down, up_key_down, down_key_down);
		_renderer.draw();

		//�X���b�v
		glfwSwapBuffers(_window);

		//�C�x���g�̃|�[�����O
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
	height = 800;
	width = 1200;
}
Application::~Application()
{

}