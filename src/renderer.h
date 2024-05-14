#pragma once
#include <string>

using namespace std;

class renderer
{
private:
	bool setShaderFile();
public:
	bool setRenderer();
	renderer();
	~renderer();
};