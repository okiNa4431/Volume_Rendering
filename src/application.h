#pragma once

#include <iostream>
#include <include/GL/glew.h>
#include <glfw3.h>
#include "renderer.h"

class Application
{
private:
	//ウィンドウ
	GLFWwindow* _window;

	//renderer
	renderer _renderer;

	//シングルトンなのでコンストラクタをここに置いておく
	Application();
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;
public:
	//シングルトンインスタンスを返す
	static Application& Instance();

	bool Init();
	void Run();
	void Terminate();
	~Application();
};