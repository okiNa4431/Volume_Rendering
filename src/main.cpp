#include "application.h"

using namespace std;

int main()
{
	auto& app = Application::Instance();
	if (!app.Init())
	{
		exit(1);
	}
	app.Run();
	app.Terminate();
	return 0;
}