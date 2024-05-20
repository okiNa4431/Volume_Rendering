#pragma once
#include <string>
#include <include/GL/glew.h>

using namespace std;

class renderer
{
private:
	bool setShaderFile(string& vertex, string& fragment);
	int compile_shader(const string& shader_str, const int shader_type);
	GLuint _VA0, _VB0;
public:
	bool setRenderer();
	bool setObject();
	bool setVolume(const string& filePath);
	void draw();
	renderer();
	~renderer();
};