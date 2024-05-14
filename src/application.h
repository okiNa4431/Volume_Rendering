#pragma once

#include <iostream>
#include <glfw3.h>

class Application
{
private:
	//ウィンドウ
	GLFWwindow* window;

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