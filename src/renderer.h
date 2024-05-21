#pragma once
#include <string>
#include <include/GL/glew.h>

using namespace std;

class renderer
{
private:
	bool setShaderFile(string& vertex, string& fragment);
	int compile_shader(const string& shader_str, const int shader_type);
	bool setScreen();
	GLuint _VA0, _VB0;//���_�o�b�t�@�A���_�f�[�^
	int _programId;//�V�F�[�_�[�I�u�W�F�N�g
	GLuint _volumeId;//�{�����[����ID
	unsigned short* _CT;//�{�����[���f�[�^
	GLfloat _camera[3];//�J�����ʒu
	GLfloat _ray[3];//���C�̕���
public:
	bool setRenderer();
	bool setVolume(const string& filePath);
	void setWorldParams(float& scrool);
	void draw();
	void terminate();
	renderer();
	~renderer();
};