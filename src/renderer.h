#pragma once
#include <string>
#include <include/GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class renderer
{
private:
	bool setShaderFile(string& vertex, string& fragment);
	int compile_shader(const string& shader_str, const int shader_type);
	bool setCube();
	GLuint _VA0, _VB0,_IB0;//���_�f�[�^�A���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@
	int _programId;//�V�F�[�_�[�I�u�W�F�N�g
	GLuint _volumeId;//�{�����[����ID
	unsigned short* _CT;//�{�����[���f�[�^
	vec3 _camera;//�J�����ʒu
	vec3 _source;//�I�u�W�F�N�g�̈ʒu
	vec3 _target;//�����_
	vec3 _up;//��x�N�g��
	GLfloat _close;//�����_�ƃJ�����̋���

public:
	bool setRenderer();
	bool setVolume(const string& filePath);
	void setWorldParams(float& scrool, float deltaX, float deltaY, bool rotateF, bool translateF);
	void draw();
	void terminate();
	renderer();
	~renderer();
};