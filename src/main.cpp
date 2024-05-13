#include "Volume_Rendering.h"
#include <glfw3.h>

using namespace std;

int main()
{
	// GLFWの初期化
	if (!glfwInit()) {
		return -1;
	}

	// ウィンドウを作成
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// カレントのウィンドウを設定
	glfwMakeContextCurrent(window);

	// メインループ
	while (!glfwWindowShouldClose(window)) {
		// ウィンドウのクリア
		glClear(GL_COLOR_BUFFER_BIT);

		// ダブルバッファのスワップ
		glfwSwapBuffers(window);

		// イベントのポーリング
		glfwPollEvents();
	}

	// GLFWの終了
	glfwTerminate();

	return 0;
}