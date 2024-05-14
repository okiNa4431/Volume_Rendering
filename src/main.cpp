﻿#include "application.h"

using namespace std;

int main()
{
	auto& app = Application::Instance();
	if (!app.Init())
	{
		exit(EXIT_FAILURE);
	}
	app.Run();
	app.Terminate();
	return 0;
}