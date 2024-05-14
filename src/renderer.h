#pragma once
#include <string>

using namespace std;

class renderer
{
private:
	bool setShaderFile(const string& vertexFileName, const string& fragmentFileName);
public:
	renderer();
	~renderer();
};