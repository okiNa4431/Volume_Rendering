#pragma once
#include <string>

using namespace std;

class renderer
{
private:
	bool setShaderFile(string& vertex, string& fragment);
	int compile_shader(const string& shader_str, const int shader_type);
public:
	bool setRenderer();
	renderer();
	~renderer();
};