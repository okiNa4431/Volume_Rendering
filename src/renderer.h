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
	int _programId;
public:
	bool setRenderer();
	bool setScreen();
	bool setVolume(const string& filePath);
	void draw();
	void terminate();
	renderer();
	~renderer();
};